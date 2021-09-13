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

#include "room/RoomManager.hpp"
#include "room/interpreter/parser.hpp"
#include "room/interpreter/token.hpp"
#include "display_server.hpp"
#include "fileio/fileio.h"
#include "game_error.hpp"
#include "pcurses.hpp"
#include "rendering.hpp"
#include "stringsm.h"
#include "userio.hpp"

static bool is_a_comment(std::string const& p_str)
{
    if(p_str[0] == '#') return true;
    else return false;
}

bool RoomManager::set_room_property(PStrings const& pstrings,
        std::string const& room_name, std::string const& prop_name,
        std::string const& prop_arg, room_property_rtrn& return_variables)
{
    if(return_variables.use_flag) {
        game_error::fatal_error("Multiple " + prop_name + " properties in " + room_name + " ROOM");
        return false;
    } else if(stringsm::has_quotes(prop_arg)) {
        return_variables.use_flag = true;
        return_variables.str = stringsm::ext_str_quotes(prop_arg);
    } else {
        return_variables.use_flag = true;
        return_variables.str = pstrings.fetch(prop_arg);
    }
    return true;
}

bool RoomManager::set_CHOICES(PStrings const& pstrings,
        std::string const& room_name, RoomVectorData& vec_data,
        RoomCHOICESData& p_data)
{
    if(p_data.use_flag) {
        game_error::fatal_error("More than one CHOICES block in " + room_name + " ROOM");
        return false;
    }

    bool end_of_block = false;
    std::vector<int> defined_choices;

    while(vec_data.i < vec_data.room_file_lines.size() && !end_of_block) {
        ++vec_data.i;
        std::string& current_line = vec_data.room_file_lines[vec_data.i];

        stringsm::rtab(current_line);

        if(current_line == "" || is_a_comment(current_line)) continue;
        else if(current_line[0] == 'c' && isdigit(current_line[1]) && current_line[2] == '\0') {
            int new_choice_id = current_line[1] - '0';

            if(std::find(defined_choices.cbegin(), defined_choices.cend(), new_choice_id)
                    != defined_choices.cend()) {
                game_error::fatal_error("Choice " + std::to_string(new_choice_id) + " defined"
                        + " multiple times in " + room_name + " ROOM");
                return false;
            }

            bool filler_use_flag = false;
            std::vector<TokenVec> new_choice_ins;
            RoomBlockData block_data { new_choice_ins, "CHOICE" + std::to_string(new_choice_id),
                filler_use_flag };
            if(!set_block(pstrings, room_name, vec_data, block_data)) {
                return false;
            }

            Choice new_choice(new_choice_id, std::move(new_choice_ins),
                    pstrings);

            if(game_error::has_encountered_fatal()) {
                return false;
            } else {
                p_data.choices_vec.push_back(std::move(new_choice));
            }
        }
        else if(stringsm::getfw(current_line) == "END") {
            end_of_block = true;
        } else {
            game_error::fatal_error("Non-Choice field in " + room_name
                    + " ROOM CHOICES:" + current_line);
            return false;
        }
    }

    if(!end_of_block) {
        game_error::fatal_error("EOF before end of CHOICES block in "
                + room_name + " ROOM");
        return false;
    }

    p_data.use_flag = true;
    return true;

}

bool RoomManager::set_block(PStrings const& pstrings,
        std::string const& room_name, RoomVectorData& vec_data,
        RoomBlockData& block_data)
{
    if(block_data.use_flag) {
        game_error::fatal_error(block_data.block_name
                + " block was redefined in " + room_name + " ROOM");
        return false;
    }

    bool end_of_block = false;

    while(vec_data.i < vec_data.room_file_lines.size() && !end_of_block) {
        ++vec_data.i;

        std::string& current_line = vec_data.room_file_lines[vec_data.i];

        stringsm::rtab(vec_data.room_file_lines[vec_data.i]);

        if(current_line == "" || is_a_comment(current_line)) {
            continue;
        } else if(stringsm::getfw(current_line) == "IF") {
            TokenVec new_tknvec = token::create_arr(
                    vec_data.room_file_lines[vec_data.i],
                    pstrings);
            block_data.block_ins.push_back(std::move(new_tknvec));

            bool block_flag = false;
            RoomBlockData if_block_data { block_data.block_ins,
                block_data.block_name, block_flag };

            if(!set_block(pstrings, room_name, vec_data, if_block_data)) {
                return false;
            }

            TokenVec end_tokenvec { { "END", token_type::END,
                token_spec_type::NONE } };
            block_data.block_ins.push_back(end_tokenvec);
        } else if(stringsm::getfw(current_line) == "ELSE") {
            if(!stringsm::is_single_word(current_line)) {
                game_error::fatal_error("ELSE followed by other tokens (" + current_line + ")");
                return false;
            }

            TokenVec new_tknvec = token::create_arr(
                    vec_data.room_file_lines[vec_data.i], pstrings);
            block_data.block_ins.push_back(std::move(new_tknvec));

            bool block_flag = false;
            RoomBlockData else_block_data { block_data.block_ins, block_data.block_name,
                block_flag };

            if(!set_block(pstrings, room_name, vec_data, else_block_data)) {
                return false;
            }
            TokenVec end_tokenvec { { "END", token_type::END,
                token_spec_type::NONE } };
            block_data.block_ins.push_back(end_tokenvec);
        } else if(stringsm::getfw(current_line) == "END") {
            end_of_block = true;
        } else if(!is_a_comment(vec_data.room_file_lines[vec_data.i])) {
            TokenVec new_tknvec = token::create_arr(
                    vec_data.room_file_lines[vec_data.i], pstrings);
            block_data.block_ins.push_back(std::move(new_tknvec));
        }
    }

    if(!end_of_block) {
        game_error::fatal_error("EOF before end of " + block_data.block_name + "block in "
                + room_name + " ROOM");
        return false;
    }

    block_data.use_flag = true;
    return true;
}

Room RoomManager::create_new_room(PStrings const& pstrings,
        std::vector<std::string> room_file_lines, unsigned int& i,
        bool& no_error, std::string const& room_name)
{
    bool end_of_room = false;
    bool has_title = false;
    bool has_desc = false;
    bool has_atlaunch = false;
    bool has_choices = false;
    std::string room_title;
    std::string room_desc;
    std::vector<TokenVec> atlaunch_ins;
    std::vector<Choice> Choices_vec;

    no_error = false;

    while(i < room_file_lines.size() && !end_of_room && !game_error::has_encountered_fatal()) {
        ++i;
        std::string ins_type;
        std::string ins_arg;
        std::string& current_line = room_file_lines[i];

        stringsm::rtab(current_line);
        parser::splitline(ins_type, ins_arg, current_line);

        if(ins_type == "TITLE") {
            room_property_rtrn title_rtrn { room_title, has_title };

            if(!set_room_property(pstrings, room_name, "TITLE", ins_arg,
                        title_rtrn)) {
                return Room();
            }
        } else if(ins_type == "DESC") {
            room_property_rtrn desc_rtrn { room_desc, has_desc };

            if(!set_room_property(pstrings, room_name, "DESC", ins_arg,
                        desc_rtrn)) {
                return Room();
            }
        } else if(ins_type == "ATLAUNCH") {
            RoomBlockData atlaunch_data { atlaunch_ins, "ATLAUNCH",
                has_atlaunch };
            RoomVectorData vec_data { room_file_lines, i };

            if(!set_block(pstrings, room_name, vec_data, atlaunch_data)) {
                return Room();
            }
        } else if(ins_type == "CHOICES") {
            RoomCHOICESData CHOICES_data { Choices_vec, has_choices };
            RoomVectorData vec_data { room_file_lines, i };

            if(!set_CHOICES(pstrings, room_name, vec_data, CHOICES_data)) {
                return Room();
            }
        } else if(ins_type == "END") {
            end_of_room = true;
        } else if(!is_a_comment(current_line)) {
            game_error::fatal_error("Unknown instruction in " + room_name
                    + " ROOM: " + current_line);
            return Room();
        }
    }

    if(!end_of_room) {
        game_error::fatal_error("EOF before end of " + room_name + " ROOM");
        return Room();
    }
    no_error = true;

    if(has_title && has_desc) {
        Room new_room(room_name, room_title, room_desc);
        if(has_choices) new_room.setChoices(std::move(Choices_vec));
        if(has_atlaunch) new_room.setATLAUNCH_ins(std::move(atlaunch_ins));
        return new_room;
    } else if(has_title) {
        Room new_room(room_name, room_title);
        if(has_choices) new_room.setChoices(std::move(Choices_vec));
        if(has_atlaunch) new_room.setATLAUNCH_ins(std::move(atlaunch_ins));
        return new_room;
    } else if(has_desc) {
        Room new_room(room_name);
        new_room.setDesc(room_desc);
        if(has_choices) new_room.setChoices(std::move(Choices_vec));
        if(has_atlaunch) new_room.setATLAUNCH_ins(std::move(atlaunch_ins));
        return new_room;
    } else {
        Room new_room;
        if(has_choices) new_room.setChoices(std::move(Choices_vec));
        if(has_atlaunch) new_room.setATLAUNCH_ins(std::move(atlaunch_ins));
        return new_room;
    }
}

RoomManager::RoomManager(PStrings const& pstrings,
        std::filesystem::path const& room_file_path)
{
    std::vector<std::string> room_file_lines = fileio::copy_to_vector(room_file_path);

    for(unsigned int i = 0; i < room_file_lines.size() && !game_error::has_encountered_fatal();
            ++i) {
        std::string& current_line = room_file_lines[i];

        stringsm::rtab(current_line);
        if(current_line == "" || current_line[0] == '#') continue;

        std::string ins_type;
        std::string ins_arg;
        parser::splitline(ins_type, ins_arg, room_file_lines[i]);

        if(ins_type != "ROOM") {
            game_error::fatal_error("Instruction before ROOM definition: " + current_line);
            break;
        }

        if(!stringsm::is_single_word(ins_type)) {
            game_error::fatal_error("ROOM name must not contain any spaces or tabs: "
                    + current_line);
            break;
        }

        bool room_no_error;
        Room new_room = create_new_room(pstrings,
                room_file_lines, i, room_no_error, ins_arg);

        if(!room_no_error) break;
        else if(!m_room_map.insert(std::make_pair(new_room.getName(), new_room)).second) {
            game_error::fatal_error("Duplicate room in rooms file: " + new_room.getName());
            break;
        }
    }
}

void RoomManager::startLoop(PStrings const& pstrings,
        CutscenesContainer const& cs_container, game_state_s& game_state,
        std::string const& start_room)
{
    std::string current_room_id = start_room;

    while(!game_state.should_game_exit) {
        auto const& room_it = m_room_map.find(current_room_id.c_str());

        if(room_it == m_room_map.cend()) {
            game_error::fatal_error(current_room_id + " ROOM not present in rooms file");
            return;
        }

        Room const& currentRoom = room_it->second;
        m_rls.resetGameOver();

        if(!currentRoom.load(pstrings, m_room_map, cs_container, m_player,
                    m_rls, game_state)) {
            break;
        }
        if(!game_state.should_game_exit) {
            current_room_id = game_state.next_room;
        }
    }
    display_server::clear_screen();
}
