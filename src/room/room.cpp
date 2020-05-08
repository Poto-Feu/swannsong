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
#include "vars/pvars.h"
#include "vars/pconst.h"
#include "pstrings.h"
#include "perror.h"
}

#include <cstring>
#include <string>
#include "room.h"
#include "room_io.h"
#include "find.hpp"
#include "interpreter/parser.h"
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
        char arg[P_MAX_BUF_SIZE - 1] = {0};
        char type[P_MAX_BUF_SIZE - 1] = {0};
        char temp_buf[P_MAX_BUF_SIZE] = {0};
        std::string buf;

        roomio_fetch_ln(buf, currln);
        strcpy(temp_buf, buf.c_str());
        parser_splitline(type, arg, temp_buf);

        if(!strcmp(type, "TEXT"))
        {
            textfound = true;
            printw("%d. ", choice_n);

            if(stringsm_is_str(arg))
            {
                std::string disp_value;

                stringsm_ext_str_quotes(disp_value, arg);
                printw(disp_value.c_str());
            }
            else pstrings_display(arg);

            printw("\n");

            currln++;
        }
        else if(!strcmp(type, "END")) perror_disp("missing choice text", true);
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
    auto title_shown = false;
    auto choice_n = 1;

    for(auto i = 0; i < static_cast<decltype(i)>(display_order.size()); i++)
    {
        if(display_order[i] == CHOICE)
        {
            displayed_choices[choice_n - 1].display();
            choice_n++;
        } else if(display_order[i] == TITLE) displayTitle(title_shown);
    }
}

void Room::addDisplayTitle()
{
    display_order.push_back(TITLE);
}

void Room::addDisplayDesc()
{
    display_order.push_back(DESC);
}

void Room::addDisplayChoice(int ch_ln)
{
    Choice newChoice(displayed_choices.size() + 1, ch_ln);
    displayed_choices.push_back(newChoice);
    display_order.push_back(CHOICE);
}

/*Choice private member functions definitions*/
void Room::displayTitle(bool& title_shown)
{
    std::string value;
    auto prop_fnd = find_room_property(value, "TITLE", getRoomLine());

    if(prop_fnd)
    {
        std::string disp_value;

        if(stringsm_is_str(value.c_str()))
        {
            stringsm_ext_str_quotes(disp_value, value.c_str());
        }
        else if(pstrings_check_exist(value.c_str()))
        {
            char* temp_arr = NULL;

            pstrings_fetch(value.c_str(), &temp_arr);
            disp_value = temp_arr;

            free(temp_arr);
        }

        attron(A_BOLD);
        printw("%s\n\n", disp_value.c_str());
        attroff(A_BOLD);

        title_shown = true;
    } else perror_disp("TITLE property not found in room", false);
}

/*Read the first ATLAUNCH block encountered starting from specified line*/
static void room_atlaunch(int roomln, Room& currentRoom)
{
    int foundln;
    bool atlfound = false;

    move(0, 0);
    clear();

    atlfound = find_atlaunchline(&foundln, roomln);
    if(atlfound == true) (void)parser_exec_until_end(foundln, currentRoom);
    refresh();
}

void room_load(char* id)
{
    int roomln = find_roomline(id);
    std::string str_id(id);

    Room currentRoom(str_id);
    
    currentRoom.setRoomLine(roomln);

    room_atlaunch(roomln, currentRoom);
    currentRoom.displayList();
}
