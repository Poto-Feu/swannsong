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

#include <algorithm>
#include <stdexcept>

#include "room/RoomState.hpp"
#include "cutscenes.hpp"
#include "display_server.hpp"
#include "game_error.hpp"
#include "game_menu.hpp"
#include "pcurses.hpp"
#include "stringsm.h"

RoomState::RoomState() { }

void RoomState::addTitle()
{
    m_title_displayed = true;
}

void RoomState::addDesc()
{
    m_desc_displayed = true;
}

void RoomState::addAllChoices()
{
    m_all_choices_displayed = true;
}

void RoomState::addChoice(unsigned int choice_n)
{
    m_choices_list.push_back(choice_n);
}

void RoomState::addString(std::string const& p_str)
{
    m_string_list.push_back(p_str);
}

void RoomState::addCutscene(std::string const& p_cs)
{
    m_cutscenes_list.push_back(p_cs);
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
    for(auto const& it : m_cutscenes_list) cutscenes::display(it, program_strings);
    m_cutscenes_list.clear();
}

std::string RoomState::displayRoomScreen(Room const& p_room, PStrings const& program_strings,
        const std::string *error_msg) const
{

    const std::string *room_title_ptr = nullptr;
    const std::string *room_desc_ptr = nullptr;
    const std::vector<std::string> *room_other_str_ptr = nullptr;
    std::string const& room_title = p_room.getTitle();
    std::string const& room_desc = p_room.getDesc();
    std::vector<std::string> room_choices_str;

    game_menu::flags menu_flags;

    if(is_title_displayed()) room_title_ptr = &room_title;
    if(is_desc_displayed()) room_desc_ptr = &room_desc;

    if(m_all_choices_displayed) {
        auto const& room_choices_vec = p_room.getChoicesVec();
        std::transform(room_choices_vec.begin(), room_choices_vec.end(),
                std::back_inserter(room_choices_str), [](Choice const& p_choice) {
                return p_choice.getText();
                });
    } else if(m_choices_list.size() != 0) {
        for(auto const& it : m_choices_list) {
            const Choice *current_choice = p_room.getChoice(it);

            if(!current_choice) {
                game_error::emit_warning(std::to_string(it) + " Choice doesn't exist in "
                        + p_room.getName() + " ROOM");
                continue;
            }

            room_choices_str.push_back(current_choice->getText());
        }
    }

    if(m_string_list.size() > 0) room_other_str_ptr = &m_string_list;
    return game_menu::display(room_title_ptr, room_desc_ptr, room_other_str_ptr, room_choices_str,
            error_msg, &menu_flags, &program_strings);
}

std::string RoomState::displayAll(Room const& p_room, PStrings const& program_strings,
        bool same_room)
{
    if(!same_room) displayCutscenes(program_strings);
    else m_cutscenes_list.clear();
    return displayRoomScreen(p_room, program_strings);
}

unsigned int RoomState::getCorrespondantChoiceId(unsigned int choice_n) const
{
    if(choice_n > m_choices_list.size() || choice_n == 0) return 0;

    return m_choices_list[choice_n - 1];
}

bool RoomState::is_title_displayed() const
{
    return m_title_displayed;
}

bool RoomState::is_desc_displayed() const
{
    return m_desc_displayed;
}

bool RoomState::is_all_choices_displayed() const
{
    return m_all_choices_displayed;
}
