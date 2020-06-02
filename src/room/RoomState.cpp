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

#include "RoomState.hpp"
#include "find.hpp"
#include "cutscenes.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "stringsm.h"

RoomState::RoomState() { }

void RoomState::reset()
{
    title_displayed = false;
    desc_displayed = false;

    choice_list.clear();
    string_list.clear();
    cs_list.clear();
}

void RoomState::addTitle()
{
    title_displayed = true;
}

void RoomState::addDesc()
{
    desc_displayed = true;
}

void RoomState::addChoice(Choice const& p_choice)
{
    choice_list.push_back(p_choice);
}

void RoomState::addString(std::string const& p_str)
{
    string_list.push_back(p_str);
}

void RoomState::addCutscene(std::string const& p_cs)
{
    cs_list.push_back(p_cs);
}

void RoomState::setBlockType(RoomState::bt const p_bt)
{
    block_type = p_bt;
}

void RoomState::displayTitle(Room const& p_room)
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

void RoomState::displayDesc(Room const& p_room)
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

void RoomState::displayChoices()
{
    for(auto& it : choice_list) it.display();
}

void RoomState::displayStrings()
{
    if(string_list.size() > 0) {
        if(!title_displayed && !desc_displayed) move(0, pcurses::title_y + 2);

        for(auto& it : string_list) pcurses::display_center_string(it);

        printw("\n\n");
    }
}

void RoomState::displayAll(Room const& p_room)
{
    displayCutscenes();

    if(is_title_displayed()) displayTitle(p_room);
    if(is_desc_displayed()) displayDesc(p_room);

    displayStrings();
    displayChoices();
}

RoomState::bt RoomState::getBlockType() const
{
    return block_type;
}

unsigned int RoomState::getChoicesSize() const
{
    return choice_list.size();
}

unsigned int RoomState::getChoiceLine(unsigned int ch_n) const
{
    if(ch_n <= choice_list.size() && ch_n != 0) {
        return choice_list[ch_n - 1].getLine();
    } else {
        perror_disp("out-of-range parameter in RoomState::getChoiceLine",
                true);
        return 0;
    }
}

void RoomState::displayCutscenes()
{
    for(auto const& it : cs_list) cutscenes::display(it);
}

bool RoomState::is_title_displayed() const
{
    return title_displayed;
}

bool RoomState::is_desc_displayed() const
{
    return desc_displayed;
}
