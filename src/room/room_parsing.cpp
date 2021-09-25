/*
    Copyright (C) 2021 Adrien Saad

    This file is part of SwannSong Adventure.

    SwannSong Adventure is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SwannSong Adventure is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong Adventure.  If not, see
    <https://www.gnu.org/licenses/>.
*/

#include <algorithm>

#include "room/room_parsing.hpp"
#include "fileio/fileio.hpp"
#include "game_error.hpp"
#include "pstrings.hpp"
#include "room/RoomClass.hpp"
#include "room/interpreter/parser.hpp"
#include "stringsm.h"

enum block_type {
    ATLAUNCH = 0,
    CHOICE = 1
};

const std::string block_type_strs[2] { "ATLAUNCH", "CHOICE" };

static bool is_a_comment(std::string const& p_str)
{
    return p_str[0] == '#';
}

bool set_block(pstrings::ps_data_ptr const& pstrings_data,
        std::string const& room_name, block_type bt,
        std::vector<std::string>& file_lines, unsigned int& line_i,
        std::vector<TokenVec>& tv_vec)
{
    bool end_of_block = false;

    while(line_i < file_lines.size() && !end_of_block) {
        const TokenVec end_tokenvec { { "END", token_type::END,
            token_spec_type::NONE } };

        ++line_i;
        stringsm::rtab(file_lines[line_i]);

        if(file_lines[line_i] == "" || is_a_comment(file_lines[line_i])) {
            continue;
        } else if(stringsm::getfw(file_lines[line_i]) == "IF") {
            TokenVec new_tknvec = token::create_arr(file_lines[line_i],
                    pstrings_data);

            tv_vec.push_back(std::move(new_tknvec));

            if(!set_block(pstrings_data, room_name, bt, file_lines, line_i,
                        tv_vec)) {
                return false;
            }

            tv_vec.push_back(end_tokenvec);
        } else if(stringsm::getfw(file_lines[line_i]) == "ELSE") {
            TokenVec new_tknvec = token::create_arr(file_lines[line_i],
                    pstrings_data);
            if(!stringsm::is_single_word(file_lines[line_i])) {
                game_error::fatal_error("ELSE followed by other tokens ("
                        + file_lines[line_i] + ")");
                return false;
            }

            tv_vec.push_back(std::move(new_tknvec));

            if(!set_block(pstrings_data, room_name, bt, file_lines, line_i,
                        tv_vec)) {
                return false;
            } else {
                tv_vec.push_back(end_tokenvec);
            }
        } else if(stringsm::getfw(file_lines[line_i]) == "END") {
            end_of_block = true;
        } else if(!is_a_comment(file_lines[line_i])) {
            TokenVec new_tknvec = token::create_arr(file_lines[line_i],
                    pstrings_data);
            tv_vec.push_back(std::move(new_tknvec));
        }
    }

    if(!end_of_block) {
        game_error::fatal_error("EOF before end of " + block_type_strs[bt]
                + "block in " + room_name + " ROOM");
        return false;
    }

    return true;
}

static void room_redef_prop_error(std::string const& room_name,
        std::string const& prop_name)
{
    game_error::fatal_error("Multiple " + prop_name + " properties in "
            + room_name + " ROOM");
}

static std::string get_property_string(pstrings::ps_data_ptr pstrings_data,
        std::string const& prop_arg)
{
    if(stringsm::has_quotes(prop_arg)) {
        return stringsm::ext_str_quotes(prop_arg);
    } else {
        return pstrings::fetch_string(pstrings_data, prop_arg);
    }
}

static bool set_CHOICES(pstrings::ps_data_ptr const& pstrings_data,
        std::string const& room_name, std::vector<std::string>& file_lines,
        unsigned int& line_i, std::vector<Choice>& choices)
{
    std::vector<int> defined_choices;
    bool end_of_block = false;

    while(line_i < file_lines.size() && !end_of_block) {
        ++line_i;

        stringsm::rtab(file_lines[line_i]);

        if(file_lines[line_i] == "" || is_a_comment(file_lines[line_i])) {
            continue;
        } else if(file_lines[line_i][0] == 'c'
                && isdigit(file_lines[line_i][1])
                && file_lines[line_i][2] == '\0') {
            std::vector<TokenVec> new_choice_ins;
            int new_choice_id = file_lines[line_i][1] - '0';

            if(std::find(defined_choices.cbegin(), defined_choices.cend(),
                        new_choice_id) != defined_choices.cend()) {
                game_error::fatal_error("Choice "
                        + std::to_string(new_choice_id) + " defined"
                        + " multiple times in " + room_name + " ROOM");
                return false;
            } else if(!set_block(pstrings_data, room_name, CHOICE, file_lines,
                        line_i, new_choice_ins)) {
                return false;
            }

            Choice new_choice(new_choice_id, std::move(new_choice_ins),
                    pstrings_data);

            if(game_error::has_encountered_fatal()) {
                return false;
            } else {
                choices.push_back(std::move(new_choice));
            }
        } else if(stringsm::getfw(file_lines[line_i]) == "END") {
            end_of_block = true;
        } else {
            game_error::fatal_error("Non-Choice field in " + room_name
                    + " ROOM CHOICES:" + file_lines[line_i]);
            return false;
        }
    }

    if(!end_of_block) {
        game_error::fatal_error("EOF before end of CHOICES block in "
                + room_name + " ROOM");
        return false;
    }

    return true;
}

bool parse_room(pstrings::ps_data_ptr const& pstrings_data,
        std::string const& room_name, std::vector<std::string>& file_lines,
        unsigned int& line_i, Room& room)
{
    bool end_of_room = false;
    bool has_title = false;
    bool has_desc = false;
    bool has_atlaunch = false;
    bool has_choices = false;
    std::string title;
    std::string desc;
    std::vector<TokenVec> atlaunch_ins;
    std::vector<Choice> choices;

    while(line_i < file_lines.size() && !end_of_room) {
        ++line_i;
        std::string ins_type;
        std::string ins_arg;

        stringsm::rtab(file_lines[line_i]);
        parser::splitline(ins_type, ins_arg, file_lines[line_i]);

        if(ins_type == "TITLE") {
            if(has_title) {
                room_redef_prop_error(room_name, "TITLE");
                return false;
            }

            has_title = true;
            title = get_property_string(pstrings_data, ins_arg);
        } else if(ins_type == "DESC") {
            if(has_desc) {
                room_redef_prop_error(room_name, "DESC");
                return false;
            }

            has_desc = true;
            desc = get_property_string(pstrings_data, ins_arg);
        } else if(ins_type == "ATLAUNCH") {
            if(has_atlaunch) {
                room_redef_prop_error(room_name, "ATLAUNCH");
                return false;
            } else if(!set_block(pstrings_data, room_name, ATLAUNCH,
                        file_lines, line_i, atlaunch_ins)) {
                return false;
            }

            has_atlaunch = true;
        } else if(ins_type == "CHOICES") {
            if(has_choices) {
                room_redef_prop_error(room_name, "CHOICES");
                return false;
            } else if(!set_CHOICES(pstrings_data, room_name, file_lines,
                        line_i, choices)) {
                return false;
            }

            has_choices = true;
        } else if(ins_type == "END") {
            end_of_room = true;
        } else if(!is_a_comment(file_lines[line_i])) {
            game_error::fatal_error("Unknown instruction in " + room_name
                    + " ROOM: " + file_lines[line_i]);
            return false;
        }
    }

    if(!end_of_room) {
        game_error::fatal_error("EOF before end of " + room_name + " ROOM");
        return false;
    }

    if(has_title) {
        room.setTitle(std::move(title));
    }

    if(has_desc) {
        room.setDesc(std::move(desc));
    }

    if(has_atlaunch) {
        room.setATLAUNCH_ins(std::move(atlaunch_ins));
    }

    if(has_choices) {
        room.setChoices(std::move(choices));
    }

    return true;
}

bool room_parsing::parse_rooms_file(pstrings::ps_data_ptr const& pstrings_data,
        std::string const& data_path,
        std::unordered_map<std::string, Room>& room_map)
{
    using namespace game_error;

    const std::string file_path = data_path + "/rooms.txt";
    std::vector<std::string> file_lines = fileio::copy_to_vector(file_path);

    for(unsigned int i = 0; i < file_lines.size()
            && !game_error::has_encountered_fatal(); ++i) {
        std::string ins_type;
        std::string ins_arg;

        stringsm::rtab(file_lines[i]);

        if(file_lines[i] == "" || file_lines[i][0] == '#') {
            continue;
        }

        parser::splitline(ins_type, ins_arg, file_lines[i]);

        if(ins_type != "ROOM") {
            fatal_error("Instruction before ROOM definition: "
                    + file_lines[i]);
            break;
        }

        if(!stringsm::is_single_word(ins_type)) {
            fatal_error("ROOM name must not contain any spaces or tabs: "
                    + file_lines[i]);
            break;
        } else {
            Room room(ins_arg);

            if(!parse_room(pstrings_data, ins_arg, file_lines, i, room)) {
                return false;
            } else if(!room_map.emplace(room.getName(), room).second) {
                fatal_error("Duplicate room in rooms file: " + ins_arg);
                break;
            }
        }
    }

    return true;
}
