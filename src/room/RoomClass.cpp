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

#include "room/RoomClass.hpp"
#include "room/interpreter/parser.hpp"
#include "room/find.hpp"
#include "room/room_struct.hpp"
#include "fileio/save/load_savefile.hpp"
#include "fileio/save/save_file.hpp"
#include "cutscenes.hpp"
#include "game_error.hpp"
#include "inventory.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "stringsm.h"
#include "userio.h"

Room::Room() { }
Room::Room(std::string const& room_name) : name(room_name) { }

std::string Room::getName() const
{
    return name;
}

bool Room::isChoicesLineSet() const
{
    if(choices_line != 0) return true;
    else return false;
}

int Room::getRoomLine() const 
{
    return room_line;
}

int Room::getChoicesLine() const 
{
    return choices_line;
}

void Room::setRoomLine(int rln) 
{
    room_line = rln;
}

void Room::setChoicesLine(int chln)
{
    choices_line = chln;
}

static void show_prompt()
{
    int str_line = display_server::get_last_line() + 2;

    if(str_line == display_server::LAST_LINE_ERR + 2) str_line = pcurses::title_y + 6;
    pcurses::display_pos_string(pstrings::fetch("room_prompt_text"), 12, str_line);
}

//Read the first ATLAUNCH block encountered starting from specified line
static bool atlaunch(room_struct& p_struct)
{
    int foundln = 0;
    bool atlfound = false;

    atlfound = room_find::atlaunchline(foundln, p_struct.currRoom.getRoomLine());

    if(atlfound) {
        p_struct.currState.setBlockType(RoomState::bt::ATLAUNCH);
        (void)parser::exec_until_end(foundln, p_struct);
        p_struct.currState.displayAll(p_struct.currRoom);
        show_prompt();
        display_server::show_screen();
        return true;
    } else {
        game_error::fatal_error("missing ATLAUNCH block (" + p_struct.currRoom.getName() + ")");
        return false;
    }
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
static bool process_input(room_struct p_struct) {
    bool correct_input = false;
    unsigned int incorrect_input_n = 0;

    display_server::save_screen();

    while(!correct_input) {
        std::string user_inp = stringsm::to_lower(userio::gettextinput(9));

        display_server::clear_screen();

        if(stringsm::is_number(user_inp)) {
            unsigned int str_digit = std::stoi(user_inp);
            unsigned int choices_n = p_struct.currState.getChoicesSize();

            if(str_digit > choices_n || str_digit == 0) incorrect_input(incorrect_input_n);
            else {
                //Process the input if it is a number corresponding to a choice
                unsigned int choice_ln = p_struct.currState.getChoiceLine(str_digit);

                p_struct.currState.setBlockType(RoomState::bt::CHOICE);
                parser::exec_until_end(choice_ln, p_struct);
                p_struct.currState.displayCutscenes();
                if(p_struct.currLoopState.is_endgame()) return true;
                else correct_input = true;
            }
        } else if(user_inp == "exit") {
            correct_input = true;
            p_struct.currLoopState.endLoop();
        } else if(user_inp == "load") {
            correct_input = true;
            auto save_data = load_savefile::start_loading();

            if(save_data.file_exists && save_data.is_savefile && !save_data.is_corrupted) {
                p_struct.currLoopState.setNextRoom(save_data.room);
                inventory::replace_vector(save_data.gitem_vector);
                gvars::replace_vector(save_data.gvar_vector);
            }
        } else if(user_inp == "save") {
            correct_input = true;
            save_file::start_saving({p_struct.currRoom.getName()});
        } else if(user_inp == "help") {
            correct_input = true;
            cutscenes::display("help");
        } else if(user_inp == "inv" || user_inp == "inventory") {
            correct_input = true;
            inventory::display_screen();
        } else incorrect_input(incorrect_input_n);
    }
    return true;
}

bool Room::load(RoomLoopState& p_rls)
{
    RoomState currentState;
    room_struct p_struct { *this, currentState, p_rls };

    display_server::clear_screen();
    p_rls.setNextRoom(name);
    if(!atlaunch(p_struct)) return false;
    else if(!p_rls.is_endgame() && !p_rls.is_unfinished()) {
        process_input(p_struct);
    }

    return true;
}
