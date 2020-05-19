/*
    Copyright (C) 2020 Adrien Saad

    This file is part of SwannSong.

    SwannSong is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SwannSong is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong.  If not, see <https://www.gnu.org/licenses/>.
*/

extern "C" {
#include <curses.h>
#include "perror.h"
}

#include <cstring>
#include <string>
#include "room.hpp"
#include "room_io.h"
#include "find.hpp"
#include "interpreter/parser.h"
#include "vars/pconst.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "stringsm.h"


/*Choice constructor definition*/
Choice::Choice(int ch_n, int ch_ln) : choice_n(ch_n), choice_line(ch_ln) { }

/*Choice public member functions definitions*/
void Choice::display()
{
    bool textfound = false;
    int currln = choice_line + 1;

    for(int i = 0; !textfound; i++)
    {
        int x = 0;
        int y = 0;
        std::string buf;
        std::string type;
        std::string arg;
        std::string disp_value;

        getyx(stdscr, y, x);

        roomio::fetch_ln(buf, currln);
        parser::splitline(type, arg, buf);

        if(type == "TEXT")
        {
            textfound = true;

            if(stringsm::is_str(arg))
            {
                disp_value = stringsm::ext_str_quotes(arg);
            } else
            {
                disp_value = pstrings::fetch(arg);
                disp_value.insert(0, ". ");
                disp_value.insert(0, std::to_string(choice_n));
            }

            move(y, 0);
            pcurses::display_pos_string(disp_value, 4);
            printw("\n");

            currln++;
        }
        else if(type == "END") perror_disp("missing choice text", true);
    }
}

/*Room constructor definition*/
Room::Room(std::string room_name) : name(room_name) { }

/*Room methods definitions*/
void Room::getName(char* r_name) const { strcpy(r_name, name.c_str()); }

bool Room::isRoomLineSet() const
{
    if(room_line != 0) return true;
    else return false;
}

bool Room::isChoicesLineSet() const
{
    if(choices_line != 0) return true;
    else return false;
}

int Room::getRoomLine() const { return room_line; }

int Room::getChoicesLine() const { return choices_line; }

void Room::setRoomLine(int rln) { room_line = rln; }

void Room::setChoicesLine(int chln) { choices_line = chln; }

void Room::displayList()
{
    if(title_displayed) displayTitle();
    if(desc_displayed) displayDesc();

    for(auto& ch : displayed_choices) ch.display();
}

void Room::addDisplayTitle() { title_displayed = true; }

void Room::addDisplayDesc() { desc_displayed = true; }

void Room::addDisplayChoice(int ch_ln)
{
    Choice newChoice(displayed_choices.size() + 1, ch_ln);
    displayed_choices.push_back(newChoice);
}

/*Choice private member functions definitions*/
void Room::displayTitle()
{
    std::string value;
    bool prop_fnd = room_find::room_property(value, "TITLE", getRoomLine());

    if(prop_fnd)
    {
        int y = pcurses::title_y;
        std::string disp_value;

        if(stringsm::is_str(value))
        {
            disp_value = stringsm::ext_str_quotes(value);
        } else if(pstrings::check_exist(value))
        {
            disp_value = pstrings::fetch(value);
        }

        attron(A_BOLD);
        move(y, getcurx(stdscr));
        pcurses::display_center_string(disp_value);
        printw("\n\n");
        attroff(A_BOLD);
    } else perror_disp("TITLE property not found in room", false);
}

void Room::displayDesc()
{
    std::string value;
    auto prop_fnd = room_find::room_property(value, "DESC", getRoomLine());

    if(prop_fnd)
    {
        int y = getcury(stdscr);
        std::string disp_value;

        if(stringsm::is_str(value))
        {
            disp_value = stringsm::ext_str_quotes(value);
        }
        else if(pstrings::check_exist(value))
        {
            disp_value = pstrings::fetch(value);
        }

        if(!title_displayed) y = pcurses::title_y + 2;

        move(y, getcurx(stdscr));
        pcurses::display_center_string(disp_value);
        printw("\n\n");
    } else perror_disp("DESC property not found in room", false);
}

/*Read the first ATLAUNCH block encountered starting from specified line*/
static void room_atlaunch(int roomln, Room& currentRoom)
{
    int foundln;
    bool atlfound = false;

    move(0, 0);
    clear();

    atlfound = room_find::atlaunchline(foundln, roomln);
    if(atlfound == true) (void)parser::exec_until_end(foundln, currentRoom);
    refresh();
}

void room_load(std::string id)
{
    int roomln = room_find::roomline(id);
    std::string str_id(id);

    Room currentRoom(str_id);
    
    currentRoom.setRoomLine(roomln);

    room_atlaunch(roomln, currentRoom);
    currentRoom.displayList();
}
