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

extern "C" {
#include "perror.h"
}

#include <algorithm>
#include "room.hpp"
#include "find.hpp"
#include "RoomManager.hpp"
#include "room/interpreter/parser.hpp"
#include "display_server.hpp"
#include "exitgame.h"
#include "inventory.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "stringsm.h"
#include "userio.h"

static void show_prompt()
{
    int str_line = display_server::get_last_line() + 2;

    if(str_line == display_server::LAST_LINE_ERR + 2) str_line = pcurses::title_y + 6;
    pcurses::display_pos_string(pstrings::fetch("room_prompt_text"), 12, str_line);
}

//Read the first ATLAUNCH block encountered starting from specified line
static void room_atlaunch(roommod::room_struct& p_struct, RoomManager& p_rmm)
{
    int foundln = 0;
    bool atlfound = false;

    atlfound = room_find::atlaunchline(foundln, p_struct.currRoom.getRoomLine());

    if(atlfound) {
        p_struct.currState.setBlockType(RoomState::bt::ATLAUNCH);
        (void)parser::exec_until_end(foundln, p_struct, p_rmm);
        p_struct.currState.displayAll(p_struct.currRoom);
        show_prompt();
        display_server::show_screen();
    } else {
        std::string err_str = "missing ATLAUNCH block (" + p_struct.currRoom.getName() + ")";
        perror_disp(err_str.c_str(), true);
    }
}

//Process the input if it is a number corresponding to a choice
static void choice_input(unsigned int const p_inp, roommod::room_struct& p_struct,
        RoomManager& p_roomman)
{
    unsigned int choice_ln = p_struct.currState.getChoiceLine(p_inp);

    p_struct.currState.setBlockType(RoomState::bt::CHOICE);
    parser::exec_until_end(choice_ln, p_struct, p_roomman);
    p_struct.currState.displayCutscenes();

    if(p_roomman.is_endgame()) exitgame(0);
}

//Reset the room screen with an added message to notify the user that its input is not correct.
static void incorrect_input()
{
    display_server::add_string(pstrings::fetch("incorrect_input"),
            {pcurses::lines - 3, pcurses::margin}, A_BOLD);
    display_server::load_save();
    display_server::show_screen();
}

//Show the room prompt and process the input
static void process_input(roommod::room_struct p_struct, RoomManager& p_rmm)
{
    bool correct_input = false;

    display_server::save_screen();

    while(!correct_input) {
        display_server::clear_screen();
        std::string user_inp = userio::gettextinput(9);

        if(stringsm::is_number(user_inp)) {
            unsigned int str_digit = std::stoi(user_inp);
            unsigned int choices_n = p_struct.currState.getChoicesSize();

            if(str_digit > choices_n || str_digit == 0) incorrect_input();
            else {
                correct_input = true;
                choice_input(str_digit, p_struct, p_rmm);
            }
        } else if(stringsm::to_upper(user_inp) == "EXIT") {
            correct_input = true;
            p_rmm.endLoop();
        } else if(stringsm::to_upper(user_inp) == "INV"
                || stringsm::to_upper(user_inp) == "INVENTORY") {
            correct_input = true;
            inventory::display_screen();
        } else incorrect_input();
    }
}

//Load the room with the specified id
static void room_load(std::string const& p_id, RoomManager &p_rmm)
{
    static std::vector<Room> room_list;
    bool room_fnd = false;
    Room currentRoom;
    RoomState currentState;

    auto it = std::find_if(room_list.cbegin(), room_list.cend(), [p_id](Room const& crm) {
            return crm.getName() == p_id;
    });

    if(it != room_list.cend()) {
        room_fnd = true;
        currentRoom = *it;
    } else {
        int roomln = room_find::roomline(p_id);

        currentRoom = Room(p_id);
        currentRoom.setRoomLine(roomln);
    }

    roommod::room_struct p_struct { currentRoom, currentState };

    display_server::clear_screen();
    p_rmm.setNextRoom(p_id);
    room_atlaunch(p_struct, p_rmm);

    if(p_rmm.is_endgame() || p_rmm.is_unfinished()) return;

    process_input(p_struct, p_rmm);
    if(!room_fnd) room_list.push_back(currentRoom);
}

namespace roommod
{
    static void unfinished_game()
    {
        display_server::clear_screen();
        pcurses::display_center_string(pstrings::fetch("unfinished_str"), pcurses::top_margin);
    }

    //Start the game loop which loads rooms until the end signal is enabled
    void start_loop(std::string const& id)
    {
        std::string curr_room_id = id;
        RoomManager rmm;

        while(!rmm.is_endgame() && !rmm.is_unfinished()) {
            room_load(curr_room_id, rmm);
            curr_room_id = rmm.getNextRoom();
        }
        
        display_server::clear_screen();
        if(rmm.is_unfinished()) unfinished_game();
        exitgame(0);
    }
}
