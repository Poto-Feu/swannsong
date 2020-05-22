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

#include <string>
#include "room.hpp"
#include "room_io.h"
#include "find.hpp"
#include "interpreter/parser.hpp"
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

/*Room member functions definitions*/
std::string Room::getName() const
{
    return name;
}

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

/*Choice private member functions definitions*/
void DisplayManager::displayTitle(Room p_room)
{
    std::string value;
    bool prop_fnd = room_find::room_property(value, "TITLE",
            p_room.getRoomLine());

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

/*DisplayManager constructor definition*/
DisplayManager::DisplayManager() { }

/*DisplayManager member functions definitions*/
void DisplayManager::addTitle()
{
    title_displayed = true;
}

void DisplayManager::addDesc()
{
    desc_displayed = true;
}

void DisplayManager::addChoice(Choice p_choice)
{
    choice_list.push_back(p_choice);
}

void DisplayManager::addString(std::string p_str)
{
    string_list.push_back(p_str);
}

void DisplayManager::addCutscene(std::string const p_cs)
{
    cs_list.push_back(p_cs);
}

void DisplayManager::displayDesc(Room p_room)
{
    std::string value;
    auto prop_fnd = room_find::room_property(value, "DESC",
            p_room.getRoomLine());

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

        if(!is_title_displayed()) y = pcurses::title_y + 2;

        move(y, getcurx(stdscr));
        pcurses::display_center_string(disp_value);
        printw("\n\n");
    } else perror_disp("DESC property not found in room", false);
}

void DisplayManager::displayStrings()
{
    if(string_list.size() > 0)
    {
        if(!title_displayed && !desc_displayed) move(0, pcurses::title_y + 2);

        for(auto& it : string_list)
        {
            pcurses::display_center_string(it);
        }

        printw("\n\n");
    }
}

void DisplayManager::displayCutscenes()
{
    for(auto const& it : cs_list)
    {
        cutscenes::display(it);
    }
}

void DisplayManager::displayChoices()
{
    for(auto& it : choice_list) it.display();
}

bool DisplayManager::is_title_displayed()
{
    if(title_displayed) return true;
    else return false;
}

bool DisplayManager::is_desc_displayed()
{
    if(desc_displayed) return true;
    else return false;
}

static void dispm_show(DisplayManager p_dispm, Room p_room)
{
    p_dispm.displayCutscenes();

    if(p_dispm.is_title_displayed()) p_dispm.displayTitle(p_room);
    if(p_dispm.is_desc_displayed()) p_dispm.displayDesc(p_room);

    p_dispm.displayStrings();
    p_dispm.displayChoices();
}

/*Read the first ATLAUNCH block encountered starting from specified line*/
static void room_atlaunch(Room& currentRoom, DisplayManager &currentDispm)
{
    int foundln;
    bool atlfound = false;

    atlfound = room_find::atlaunchline(foundln, currentRoom.getRoomLine());
    if(atlfound == true) (void)parser::exec_until_end(foundln, currentRoom,
            currentDispm);
    dispm_show(currentDispm, currentRoom);
}

/*Load the room with the specified id*/
void room_load(std::string id)
{
    int roomln = room_find::roomline(id);
    std::string str_id(id);

    Room currentRoom(str_id);
    DisplayManager currentDispm;
    
    move(0, 0);
    clear();

    currentRoom.setRoomLine(roomln);

    room_atlaunch(currentRoom, currentDispm);

    refresh();
}
