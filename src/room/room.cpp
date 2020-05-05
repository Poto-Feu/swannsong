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
#include <string.h>
#include <curses.h>
#include "vars/pvars.h"
#include "vars/pconst.h"
#include "pstrings.h"
#include "find.h"
#include "perror.h"
}

#include <string>
#include "room.h"
#include "room_io.h"
#include "interpreter/parser.h"

/*Choice constructor definition*/
Choice::Choice(int ch_n, int ch_ln) : choice_n(ch_n), choice_line(ch_ln) { }

/*Choice member functions definitions*/
void Choice::displayChoice()
{
    bool textfound = false;
    int currln = choice_line + 1;

    for(int i = 0; !textfound; i++)
    {
        char* buf = NULL;
        char arg[P_MAX_BUF_SIZE - 1] = {0};
        char type[P_MAX_BUF_SIZE - 1] = {0};

        roomio_fetch_ln(&buf, currln);
        parser_splitline(type, arg, buf);

        if(!strcmp(type, "TEXT"))
        {
            textfound = true;
            printw("%d. ", choice_n);
            pstrings_display(arg);
            printw("\n");
        }
        else if(!strcmp(type, "END"))
        {
            free(buf);
            perror_disp("missing choice text", 1);
        }
        currln++;

        free(buf);
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

void Room::displayChoices()
{
    for(auto& current_choice : displayed_choices)
    {
        current_choice.displayChoice();
    }
}

void Room::addDisplayChoice(int ch_ln)
{
    Choice newChoice(displayed_choices.size() + 1, ch_ln);
    displayed_choices.push_back(newChoice);
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
    int roomln = 0;
    std::string str_id(id);
    Room currentRoom(str_id);

    find_roomline(id, &roomln);
    room_atlaunch(roomln, currentRoom);
    currentRoom.displayChoices();
}
