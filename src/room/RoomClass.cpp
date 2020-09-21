/*
    Copyright (C) 2020 Adrien Saad

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
    along with SwannSong Adventure.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <algorithm>

#include "room/RoomClass.hpp"
#include "room/interpreter/parser.hpp"
#include "room/room_struct.hpp"
#include "fileio/save/load_savefile.hpp"
#include "fileio/save/save_file.hpp"
#include "cutscenes.hpp"
#include "game_error.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "stringsm.h"
#include "userio.h"

Room::Room() { }
Room::Room(std::string const& room_name) : m_name(room_name) { }
Room::Room(std::string const& room_name, std::string const& room_title) : m_name(room_name),
    m_title(room_title) { }
Room::Room(std::string const& room_name, std::string const& room_title,
    std::string const& room_desc) : m_name(room_name), m_title(room_title),
    m_desc(room_desc) { }

std::string const& Room::getName() const
{
    return m_name;
}

std::string const& Room::getTitle() const
{
    return m_title;
}

std::string const& Room::getDesc() const
{
    return m_desc;
}

std::vector<TokenVec> const& Room::getATLAUNCHIns() const
{
    return m_ATLAUNCH_ins;
}

std::vector<Choice> const& Room::getChoicesVec() const
{
    return m_Choices_vec;
}

Choice Room::getChoice(bool& status, unsigned int choice_n)
{
    auto Choice_it = std::find_if(m_Choices_vec.cbegin(), m_Choices_vec.cend(),
            [&](Choice const& p_choice) {
            return choice_n == p_choice.getId();
            });

    if(Choice_it == m_Choices_vec.cend()) {
        status = false;
        return Choice();
    } else {
        status = true;
        return *Choice_it;
    }
}

void Room::setDesc(std::string const& room_desc)
{
    m_desc = room_desc;
}

void Room::setATLAUNCH_ins(std::vector<TokenVec>&& atlaunch_ins)
{
    m_ATLAUNCH_ins = std::move(atlaunch_ins);
}

void Room::setChoices(std::vector<Choice>&& choices_vec)
{
    m_Choices_vec = std::move(choices_vec);
}

static void show_prompt()
{
    int str_line = display_server::get_last_line() + 2;

    if(str_line == display_server::LAST_LINE_ERR + 2) str_line = pcurses::title_y + 6;
    pcurses::display_pos_string(pstrings::fetch("room_prompt_text"), 12, str_line);
}

//Read the first ATLAUNCH block encountered starting from specified line
static void atlaunch(room_struct& p_struct)
{
    unsigned int foundln = 0;

    p_struct.currState.setBlockType(RoomState::bt::ATLAUNCH);
    parser::exec_until_end(p_struct.currRoom.getATLAUNCHIns(), p_struct, foundln);
    p_struct.currState.displayAll(p_struct.currRoom);
    show_prompt();
    display_server::show_screen();
}

//Reset the room screen with an added message to notify the user that its input is not correct
static void incorrect_input(unsigned int& incorrect_input_n)
{
    if(incorrect_input_n < 3) {
        ++incorrect_input_n;
        display_server::clear_screen();
        display_server::add_string(pstrings::fetch("incorrect_input"),
                {pcurses::lines - 3, pcurses::margin}, A_BOLD);
        display_server::load_save();
        display_server::show_screen();
    } else {
        display_server::clear_screen();
        display_server::add_string(pstrings::fetch("room_need_help"),
                {pcurses::lines - 3, pcurses::margin}, A_BOLD);
        display_server::load_save();
        display_server::show_screen();
    }
}

//Show the room prompt and process the input
static bool process_input(room_struct& p_struct) {
    bool correct_input = false;
    unsigned int incorrect_input_n = 0;

    display_server::save_screen();

    while(!correct_input) {
        std::string user_inp = stringsm::to_lower(userio::gettextinput(9));

        display_server::clear_screen();

        if(stringsm::is_number(user_inp)) {
            bool choice_status = false;
            unsigned int str_digit = std::stoi(user_inp);
            Choice const& current_choice = p_struct.currRoom.getChoice(choice_status, str_digit);

            if(!choice_status || str_digit == 0) incorrect_input(incorrect_input_n);
            else {
                //Process the input if it is a number corresponding to a choice
                unsigned int start_ln = 0;

                p_struct.currState.setBlockType(RoomState::bt::CHOICE);
                parser::exec_until_end(current_choice.getInstructions(), p_struct, start_ln);
                if(game_error::has_encountered_fatal()) return false;
                else {
                    p_struct.currState.displayCutscenes();
                    if(p_struct.currLoopState.is_endgame()) return true;
                    else correct_input = true;
                }
            }
        } else if(user_inp == "exit") {
            correct_input = true;
            p_struct.currLoopState.endLoop();
        } else if(user_inp == "load") {
            correct_input = true;
            auto save_data = load_savefile::start_loading();

            if(save_data.file_exists && save_data.is_savefile && !save_data.is_corrupted) {
                p_struct.currLoopState.setNextRoom(save_data.room);
                p_struct.currPlayer.inv = std::move(save_data.inv);
                gvars::replace_vector(save_data.gvar_vector);
            }
        } else if(user_inp == "save") {
            correct_input = true;
            save_file::start_saving({p_struct.currRoom.getName(), p_struct.currPlayer});
        } else if(user_inp == "help") {
            correct_input = true;
            cutscenes::display("help");
        } else if(user_inp == "inv" || user_inp == "inventory") {
            correct_input = true;
            inventory::display_screen(p_struct.currPlayer.inv);
        } else incorrect_input(incorrect_input_n);
    }
    return true;
}

bool Room::load(RoomLoopState& p_rls, Player& p_player,
        std::unordered_map<std::string, Room> const& room_map)
{
    RoomState currentState;
    room_struct p_struct { *this, currentState, p_rls, p_player, room_map };

    display_server::clear_screen();
    p_rls.setNextRoom(m_name);

    atlaunch(p_struct);
    if(game_error::has_encountered_fatal()) return false;
    else if(!p_rls.is_endgame() && !p_rls.is_unfinished()) process_input(p_struct);

    if(game_error::has_encountered_fatal()) return false;
    else return true;
}
