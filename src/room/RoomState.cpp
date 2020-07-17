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
#include "room/find.hpp"
#include "cutscenes.hpp"
#include "display_server.hpp"
#include "game_error.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
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
    bool prop_fnd = room_find::room_property(value, "TITLE", p_room.getRoomLine());

    if(prop_fnd) {
        std::string disp_value;

        if(stringsm::is_str(value)) disp_value = stringsm::ext_str_quotes(value);
        else if(pstrings::check_exist(value)) disp_value = pstrings::fetch(value);
        else throw std::runtime_error("unknown string format in displayTitle");

        pcurses::display_center_string(disp_value, pcurses::title_y, A_BOLD);
    } else game_error::emit_warning("TITLE property not found in room");
}

void RoomState::displayDesc(Room const& p_room)
{
    std::string value;
    auto prop_fnd = room_find::room_property(value, "DESC", p_room.getRoomLine());

    if(prop_fnd) {
        int str_line = 1;
        std::string disp_value;

        if(title_displayed) str_line = display_server::get_last_line() + 2;
        else str_line = pcurses::title_y + 2;

        if(stringsm::is_str(value)) disp_value = stringsm::ext_str_quotes(value);
        else if(pstrings::check_exist(value)) disp_value = pstrings::fetch(value);
        else throw std::runtime_error("unknown string format in displayDesc (" + value + ")");

        pcurses::display_center_string(disp_value, str_line);
    } else game_error::emit_warning("DESC property not found in room");
}

void RoomState::displayChoices()
{
    if(title_displayed || desc_displayed || !string_list.empty()) {
        pcurses::display_center_string("", display_server::get_last_line() + 1);
    }
    for(auto& it : choice_list) it.display();
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

void RoomState::displayAll(Room const& p_room)
{
    displayCutscenes();
    display_server::clear_screen();

    if(is_title_displayed()) displayTitle(p_room);
    if(is_desc_displayed()) displayDesc(p_room);

    if(string_list.size() > 0) displayStrings();
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
    if(ch_n <= choice_list.size() && ch_n != 0) return choice_list[ch_n - 1].getLine();
    else {
        game_error::fatal_error("out-of-range parameter in RoomState::getChoiceLine");
        return 0;
    }
}

void RoomState::displayCutscenes()
{
    for(auto const& it : cs_list) cutscenes::display(it);
    cs_list.clear();
}

bool RoomState::is_title_displayed() const
{
    return title_displayed;
}

bool RoomState::is_desc_displayed() const
{
    return desc_displayed;
}
