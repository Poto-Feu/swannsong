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
#include <curses.h>
#include "perror.h"
}

#include <algorithm>
#include <string>
#include "room.hpp"
#include "room_io.h"
#include "find.hpp"
#include "interpreter/parser.hpp"
#include "exitgame.h"
#include "inventory.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "stringsm.h"
#include "userio.h"

//Choice constructor definition
Choice::Choice(int ch_n, int ch_ln) : choice_n(ch_n), choice_line(ch_ln) { }

//Choice public member functions definitions
void Choice::display() const
{
    bool textfound = false;
    int currln = choice_line + 1;

    for(int i = 0; !textfound; i++) {
        int x = 0;
        int y = 0;
        std::string buf;
        std::string type;
        std::string arg;
        std::string disp_value;

        getyx(stdscr, y, x);
        roomio::fetch_ln(buf, currln);
        parser::splitline(type, arg, buf);

        if(type == "TEXT") {
            textfound = true;

            if(stringsm::is_str(arg)) {
                disp_value = stringsm::ext_str_quotes(arg);
            } else {
                disp_value = pstrings::fetch(arg);
                disp_value.insert(0, ". ");
                disp_value.insert(0, std::to_string(choice_n));
            }

            move(y, 0);
            pcurses::display_pos_string(disp_value, pcurses::choice_space);
            printw("\n");

            currln++;
        }
        else if(type == "END") perror_disp("missing choice text", true);
    }
}

unsigned int Choice::getLine() const
{
    return choice_line;
}

//Room constructors definitions
Room::Room() { }
Room::Room(std::string const& room_name) : name(room_name) { }

//Room member functions definitions
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

/*RoomManager constructor definition*/
RoomManager::RoomManager() { }

/*Set the variable which serve as a signal indicating if the room loop should
end*/
void RoomManager::endLoop()
{
    endgame = true;
}

void RoomManager::reset()
{
    title_displayed = false;
    desc_displayed = false;

    choice_list.clear();
    string_list.clear();
    cs_list.clear();
}

//RoomManager member functions definitions
void RoomManager::addTitle()
{
    title_displayed = true;
}

void RoomManager::addDesc()
{
    desc_displayed = true;
}

void RoomManager::addChoice(Choice const& p_choice)
{
    choice_list.push_back(p_choice);
}

void RoomManager::addString(std::string const& p_str)
{
    string_list.push_back(p_str);
}

void RoomManager::addCutscene(std::string const& p_cs)
{
    cs_list.push_back(p_cs);
}

void RoomManager::setBlockType(RoomManager::bt const p_bt)
{
    block_type = p_bt;
}

void RoomManager::setNextRoom(std::string const& p_id)
{
    next_room = p_id;
}

unsigned int RoomManager::getChoicesSize() const
{
    return choice_list.size();
}

unsigned int RoomManager::getChoiceLine(unsigned int ch_n) const
{
    if(ch_n <= choice_list.size() && ch_n != 0) {
        return choice_list[ch_n - 1].getLine();
    } else {
        perror_disp("out-of-range parameter in RoomManager::getChoiceLine",
                true);
        return 0;
    }
}

RoomManager::bt RoomManager::getBlockType() const
{
    return block_type;
}

std::string RoomManager::getNextRoom() const
{
    return next_room;
}

void RoomManager::displayTitle(Room const& p_room)
{
    std::string value;
    bool prop_fnd = room_find::room_property(value, "TITLE",
            p_room.getRoomLine());

    if(prop_fnd) {
        int y = pcurses::title_y;
        std::string disp_value;

        if(stringsm::is_str(value)) {
            disp_value = stringsm::ext_str_quotes(value);
        } else if(pstrings::check_exist(value)) {
            disp_value = pstrings::fetch(value);
        }

        attron(A_BOLD);
        move(y, getcurx(stdscr));
        pcurses::display_center_string(disp_value);
        printw("\n\n");
        attroff(A_BOLD);
    } else perror_disp("TITLE property not found in room", false);
}

void RoomManager::displayDesc(Room const& p_room)
{
    std::string value;
    auto prop_fnd = room_find::room_property(value, "DESC",
            p_room.getRoomLine());

    if(prop_fnd) {
        int y = getcury(stdscr);
        std::string disp_value;

        if(stringsm::is_str(value)) {
            disp_value = stringsm::ext_str_quotes(value);
        } else if(pstrings::check_exist(value)) {
            disp_value = pstrings::fetch(value);
        }

        if(!is_title_displayed()) y = pcurses::title_y + 2;

        move(y, getcurx(stdscr));
        pcurses::display_center_string(disp_value);
        printw("\n\n");
    } else perror_disp("DESC property not found in room", false);
}

void RoomManager::displayChoices()
{
    for(auto& it : choice_list) it.display();
}

void RoomManager::displayStrings()
{
    if(string_list.size() > 0) {
        if(!title_displayed && !desc_displayed) move(0, pcurses::title_y + 2);

        for(auto& it : string_list) pcurses::display_center_string(it);

        printw("\n\n");
    }
}

void RoomManager::displayCutscenes()
{
    for(auto const& it : cs_list) cutscenes::display(it);
}

bool RoomManager::is_endgame() const
{
    return endgame;
}

bool RoomManager::is_title_displayed() const
{
    return title_displayed;
}

bool RoomManager::is_desc_displayed() const
{
    return desc_displayed;
}

//Display the current room according to the RoomManager data
static void roomman_show(RoomManager p_roomman, Room const& p_room)
{
    p_roomman.displayCutscenes();

    if(p_roomman.is_title_displayed()) p_roomman.displayTitle(p_room);
    if(p_roomman.is_desc_displayed()) p_roomman.displayDesc(p_room);

    p_roomman.displayStrings();
    p_roomman.displayChoices();
}

//Read the first ATLAUNCH block encountered starting from specified line
static void room_atlaunch(Room& currentRoom, RoomManager& p_rmm)
{
    int foundln = 0;
    bool atlfound = false;

    atlfound = room_find::atlaunchline(foundln, currentRoom.getRoomLine());

    if(atlfound) {
        p_rmm.setBlockType(RoomManager::bt::ATLAUNCH);
        (void)parser::exec_until_end(foundln, currentRoom, p_rmm);
        roomman_show(p_rmm, currentRoom);
    } else {
        std::string err_str = "missing ATLAUNCH block ("
            + currentRoom.getName() + ")";

        perror_disp(err_str.c_str(), true);
    }
}

static void show_prompt()
{
    printw("\n");
    pcurses::display_pos_string(pstrings::fetch("room_prompt_text"), 12);
}

//Process the input if it is a number corresponding to a choice
static void choice_input(unsigned int const p_inp, RoomManager& p_rmm,
        Room p_room)
{
    unsigned int choice_ln = p_rmm.getChoiceLine(p_inp);

    p_rmm.setBlockType(RoomManager::bt::CHOICE);
    parser::exec_until_end(choice_ln, p_room, p_rmm);
    p_rmm.displayCutscenes();

    if(p_rmm.is_endgame()) {
        exitgame(0);
    }

    p_rmm.reset();
}

/*Reset the room screen with an added message to notify the user that its input
is not correct.*/
static void incorrect_input(RoomManager const& p_rmm, Room const& p_room)
{
    clear();
    move(LINES - 5, pcurses::margin);
    printw("%s", (pstrings::fetch("incorrect_input")).c_str());
    roomman_show(p_rmm, p_room);
}

//Show the room prompt and process the input
static void process_input(RoomManager& p_rmm, Room const& p_room)
{
    bool correct_input = false;

    while(!correct_input) {
        show_prompt();
        refresh();

        std::string user_inp = userio::gettextinput(5);

        if(stringsm::is_number(user_inp)) {
            unsigned int str_digit = std::stoi(user_inp);
            unsigned int choices_n = p_rmm.getChoicesSize();

            if(str_digit > choices_n || str_digit == 0) {
                incorrect_input(p_rmm, p_room);
            } else {
                correct_input = true;
                choice_input(str_digit, p_rmm, p_room);
                clear();
            }
        } else if(stringsm::to_upper(user_inp) == "EXIT") {
            correct_input = true;
            p_rmm.endLoop();
            clear();
        } else if(stringsm::to_upper(user_inp) == "INV"
                || stringsm::to_upper(user_inp) == "INVENTORY") {
            correct_input = true;
            inventory::display_screen();
            clear();
            p_rmm.reset();
        } else incorrect_input(p_rmm, p_room);
    }
}

//Load the room with the specified id
static void room_load(std::string const& p_id, RoomManager &p_rmm)
{
    static std::vector<Room> room_list;
    bool room_fnd = false;
    Room currentRoom;

    auto it = std::find_if(room_list.cbegin(), room_list.cend(),
            [p_id](Room const& crm) {
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

    move(0, 0);
    clear();
    p_rmm.setNextRoom(p_id);
    room_atlaunch(currentRoom, p_rmm);

    if(p_rmm.is_endgame()) return;

    process_input(p_rmm, currentRoom);

    if(!room_fnd) room_list.push_back(currentRoom);
}

namespace roommod
{
    //Start the game loop which loads rooms until the end signal is enabled
    void start_loop(std::string const& id)
    {
        std::string curr_room_id = id;
        RoomManager rmm;

        while(!rmm.is_endgame()) {
            clear();
            room_load(curr_room_id, rmm);
            curr_room_id = rmm.getNextRoom();
        }
        exitgame(0);
    }
}
