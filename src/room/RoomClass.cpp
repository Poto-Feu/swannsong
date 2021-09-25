/*
    Copyright (C) 2021 Adrien Saad

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
    along with SwannSong Adventure.  If not, see
    <https://www.gnu.org/licenses/>.
*/

#include <algorithm>

#include "room/RoomClass.hpp"
#include "CutscenesContainer.hpp"
#include "dialogbox.hpp"
#include "game_error.hpp"
#include "game_state.hpp"
#include "player/Player.hpp"
#include "rendering.hpp"
#include "room/RoomDisplay.hpp"
#include "room/RoomLoopState.hpp"
#include "room/interpreter/parser.hpp"
#include "stringsm.h"
#include "userio.hpp"

Room::Room() { }
Room::Room(std::string const& room_name) : m_name(room_name) { }
Room::Room(std::string const& room_name, std::string const& room_title) : m_name(room_name),
    m_title(room_title) { }
Room::Room(std::string const& room_name, std::string const& room_title,
    std::string const& room_desc) : m_name(room_name), m_title(room_title),
    m_desc(room_desc) { }

std::string const& Room::getName() const
{
    return m_name;
}

std::string const* Room::getTitle() const
{
    return &m_title;
}

std::string const* Room::getDesc() const
{
    return &m_desc;
}

std::vector<TokenVec> const& Room::getATLAUNCHIns() const
{
    return m_ATLAUNCH_ins;
}

std::vector<Choice> const& Room::getChoicesVec() const
{
    return m_Choices_vec;
}

const Choice *Room::getChoice(unsigned int choice_n) const
{
    auto const& Choice_it = std::find_if(m_Choices_vec.cbegin(),
            m_Choices_vec.cend(), [=](Choice const& p_choice) {
            return choice_n == p_choice.getId();
            });

    if(Choice_it == m_Choices_vec.cend()) return nullptr;
    else return &(*Choice_it);
}

void Room::setTitle(std::string const& title)
{
    m_title = title;
}

void Room::setDesc(std::string const& room_desc)
{
    m_desc = room_desc;
}

void Room::setATLAUNCH_ins(std::vector<TokenVec>&& atlaunch_ins)
{
    m_ATLAUNCH_ins = std::move(atlaunch_ins);
}

void Room::setChoices(std::vector<Choice>&& choices_vec)
{
    m_Choices_vec = std::move(choices_vec);
}

bool Room::isChoicePresent(unsigned int choice_n) const
{
    return std::find_if(m_Choices_vec.cbegin(), m_Choices_vec.cend(),
            [=](Choice const& p_Choice) {
            return p_Choice.getId() == choice_n;
            }) != m_Choices_vec.cend();
}

static void set_game_over(Player& player, game_state_s& game_state,
        pstrings::ps_data_ptr const& pstrings_data)
{
    const std::string gameover_title = "GAME OVER";

    const std::vector<std::string> gameover_strings {
        pstrings::fetch_string(pstrings_data, "gameover_msg")
    };
    inventory::clear(player.inv);
    gvars::clear(player.gvars);
    /*Kinda bad implementation since it breaks the separation between code and data, but
    that'll do for now - the whole structure of the code is janky anyway*/
    game_state.next_room = "menu";
    dialogbox::display(&gameover_title, &gameover_strings, pstrings_data);
}

static void display_cutscenes(pstrings::ps_data_ptr const& pstrings_data,
        CutscenesContainer const& cs_container,
        std::vector<std::string> const& displayed_cutscenes)
{
    for(auto const& it : displayed_cutscenes) {
        auto const& cs = cs_container.get_cutscene(it);

        if(!cs) {
            game_error::emit_warning("Unknown cutscene");
        } else {
            rendering::display_cutscene(pstrings_data, *cs);
        }
    }
}

bool Room::load(pstrings::ps_data_ptr const& pstrings_data,
        rooms::RoomsData_ptr const& rooms_data,
        CutscenesContainer const& cs_container, Player& player,
        RoomLoopState& rls, game_state_s& game_state) const
{
    std::string const& incorrect_input_str = pstrings::fetch_string(
            pstrings_data, "incorrect_input");
    std::vector<std::string> displayed_cutscenes;
    bool wrong_input = false;

    game_state.next_room = m_name;

    do {
        RoomDisplay room_display;
        std::string menu_input;
        unsigned int atlaunch_start_ln = 0;

        if(!parser::exec_until_end(this->getATLAUNCHIns(), rooms_data, *this,
                parser::block_type::ATLAUNCH, player, rls, &room_display,
                game_state, displayed_cutscenes, atlaunch_start_ln)) {
            return false;
        }

        display_cutscenes(pstrings_data, cs_container, displayed_cutscenes);
        if(rls.is_game_over()) {
            set_game_over(player, game_state, pstrings_data);
            return true;
        }

        if(!wrong_input) {
            menu_input = rendering::display_room(pstrings_data, *this,
                    room_display);
        } else {
            menu_input = rendering::display_room(pstrings_data, *this,
                    room_display, &incorrect_input_str);
        }

        if(!userio::interpret_user_input(pstrings_data, rooms_data,
                    cs_container, *this, player, room_display, rls, game_state,
                    menu_input, wrong_input)) {
            return false;
        }

        if(rls.is_game_over()) {
            set_game_over(player, game_state, pstrings_data);
            return true;
        }
    } while(wrong_input);

    return true;
}
