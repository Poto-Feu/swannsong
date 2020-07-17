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
#include "fileio/save/load_savefile.hpp"
#include "fileio/save/save_file.hpp"
#include "exitgame.h"
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

//Read the first ATLAUNCH block encountered starting from specified line
static void atlaunch(roommod::room_struct& p_struct, RoomManager& p_rmm)
{
    int foundln = 0;
    bool atlfound = false;

    atlfound = room_find::atlaunchline(foundln, p_struct.currRoom.getRoomLine());

    if(atlfound) {
        auto show_prompt = []()
        {
            int str_line = display_server::get_last_line() + 2;

            if(str_line == display_server::LAST_LINE_ERR + 2) str_line = pcurses::title_y + 6;
            pcurses::display_pos_string(pstrings::fetch("room_prompt_text"), 12, str_line);
        };

        p_struct.currState.setBlockType(RoomState::bt::ATLAUNCH);
        (void)parser::exec_until_end(foundln, p_struct, p_rmm);
        p_struct.currState.displayAll(p_struct.currRoom);
        show_prompt();
        display_server::show_screen();
    } else {
        game_error::fatal_error("missing ATLAUNCH block (" + p_struct.currRoom.getName() + ")");
    }
}

//Reset the room screen with an added message to notify the user that its input is not correct
static void incorrect_input()
{
    display_server::add_string(pstrings::fetch("incorrect_input"),
            {pcurses::lines - 3, pcurses::margin}, A_BOLD);
    display_server::load_save();
    display_server::show_screen();
}

void Room::load(RoomManager& p_rmm)
{
    RoomState currentState;

    //Show the room prompt and process the input
    auto process_input = [this](roommod::room_struct p_struct, RoomManager& p_rmm) {
        bool correct_input = false;

        display_server::save_screen();

        while(!correct_input) {
            std::string user_inp = userio::gettextinput(9);

            display_server::clear_screen();

            if(stringsm::is_number(user_inp)) {
                unsigned int str_digit = std::stoi(user_inp);
                unsigned int choices_n = p_struct.currState.getChoicesSize();

                if(str_digit > choices_n || str_digit == 0) incorrect_input();
                else {
                    //Process the input if it is a number corresponding to a choice
                    auto choice_input = [=, &p_struct, &p_rmm]()
                    {
                        unsigned int choice_ln = p_struct.currState.getChoiceLine(str_digit);

                        p_struct.currState.setBlockType(RoomState::bt::CHOICE);
                        parser::exec_until_end(choice_ln, p_struct, p_rmm);
                        p_struct.currState.displayCutscenes();
                        if(p_rmm.is_endgame()) exitgame(0);
                    };

                    correct_input = true;
                    choice_input();
                }
            } else if(stringsm::to_upper(user_inp) == "EXIT") {
                correct_input = true;
                p_rmm.endLoop();
            } else if(stringsm::to_upper(user_inp) == "LOAD") {
                correct_input = true;
                auto save_data = load_savefile::start_loading();

                if(save_data.file_exists && save_data.is_savefile && !save_data.is_corrupted) {
                    p_rmm.setNextRoom(save_data.room);
                    inventory::replace_vector(save_data.gitem_vector);
                    gvars::replace_vector(save_data.gvar_vector);
                }
            } else if(stringsm::to_upper(user_inp) == "SAVE") {
                correct_input = true;
                save_file::start_saving({this->name});
            } else if(stringsm::to_upper(user_inp) == "INV"
                    || stringsm::to_upper(user_inp) == "INVENTORY") {
                correct_input = true;
                inventory::display_screen();
            } else incorrect_input();
        }
    };

    roommod::room_struct p_struct { *this, currentState };

    display_server::clear_screen();
    p_rmm.setNextRoom(name);
    atlaunch(p_struct, p_rmm);

    if(p_rmm.is_endgame() || p_rmm.is_unfinished()) return;

    process_input(p_struct, p_rmm);
}
