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

#include <stdexcept>

#include "room/RoomState.hpp"
#include "cutscenes.hpp"
#include "display_server.hpp"
#include "game_error.hpp"
#include "pcurses.hpp"
#include "stringsm.h"

RoomState::RoomState() { }

void RoomState::addTitle()
{
    title_displayed = true;
}

void RoomState::addDesc()
{
    desc_displayed = true;
}

void RoomState::addAllChoices()
{
    m_all_choices_displayed = true;
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

RoomState::bt RoomState::getBlockType() const
{
    return block_type;
}

void RoomState::displayCutscenes(PStrings const& program_strings)
{
    for(auto const& it : cs_list) cutscenes::display(it, program_strings);
    cs_list.clear();
}

void RoomState::displayTitle(Room const& p_room) const
{
    pcurses::display_center_string(p_room.getTitle(), pcurses::title_y, A_BOLD);
}

void RoomState::displayDesc(Room const& p_room) const
{
    int str_line;

    if(is_title_displayed()) str_line = display_server::get_last_line() + 2;
    else str_line = pcurses::title_y + 2;
    pcurses::display_center_string(p_room.getDesc(), str_line);
}

void RoomState::displayAll(Room const& p_room, PStrings const& program_strings)
{
    displayCutscenes(program_strings);
    display_server::clear_screen();

    if(is_title_displayed()) displayTitle(p_room);
    if(is_desc_displayed()) displayDesc(p_room);

    if(string_list.size() > 0) displayStrings();
    displayChoices(p_room);
}

bool RoomState::is_title_displayed() const
{
    return title_displayed;
}

bool RoomState::is_desc_displayed() const
{
    return desc_displayed;
}

void RoomState::displayChoices(Room const& p_room)
{
    if(title_displayed || desc_displayed || !string_list.empty()) {
        pcurses::display_center_string("", display_server::get_last_line() + 1);
    }

    if(m_all_choices_displayed) {
        auto choices_vec = p_room.getChoicesVec();
        for(auto& it : choices_vec) it.display();
    }
}

void RoomState::displayStrings()
{
    int str_line;
    bool firsttime = true;

    if(!title_displayed && !desc_displayed) str_line = pcurses::title_y + 2;
    else str_line = display_server::get_last_line() + 2;

    for(auto& it : string_list)
    {
        if(!firsttime) str_line = display_server::get_last_line() + 1;
        else firsttime = false;
        pcurses::display_center_string(it, str_line);
    }
}
