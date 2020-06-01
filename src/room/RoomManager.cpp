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

#include "RoomManager.hpp"
#include "find.hpp"
#include "cutscenes.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "stringsm.h"

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

