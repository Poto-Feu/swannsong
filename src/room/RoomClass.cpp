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
#include "dialogbox.hpp"
#include "game_error.hpp"
#include "player/Player.hpp"
#include "rendering.hpp"
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

std::string const& Room::getTitle() const
{
    return m_title;
}

std::string const& Room::getDesc() const
{
    return m_desc;
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
    auto const& Choice_it = std::find_if(m_Choices_vec.cbegin(), m_Choices_vec.cend(),
            [&](Choice const& p_choice) {
            return choice_n == p_choice.getId();
            });

    if(Choice_it == m_Choices_vec.cend()) return nullptr;
    else return &(*Choice_it);
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
    return std::find_if(m_Choices_vec.cbegin(), m_Choices_vec.cend(), [=](Choice const& p_Choice) {
            return p_Choice.getId() == choice_n;
            }) != m_Choices_vec.cend();
}

static void display(PStrings const& pstrings,
        std::unordered_map<std::string, Room> const& room_map,
        CutscenesContainer const& cs_container, Player& player,
        Room const& room, RoomLoopState& rls, RoomState& room_state,
        game_state_s& game_state, bool same_room)
{
    std::string menu_input = room_state.displayAll(pstrings, cs_container,
            &room.getName(), &room.getTitle(), &room.getDesc(),
            room.getChoicesVec(), same_room);
    unsigned int incorrect_input_n = 0;

    do {
    } while(!userio::interpret_user_input(pstrings, room_map, cs_container,
                player, room, rls, room_state, game_state, menu_input,
                incorrect_input_n));
}

//Read the first ATLAUNCH block encountered starting from specified line
static void atlaunch(PStrings const& pstrings,
        std::unordered_map<std::string, Room> room_map,
        CutscenesContainer const& cs_container, Player& player,
        Room const& room, RoomLoopState& rls, RoomState& room_state,
        game_state_s& game_state, bool same_room)
{
    unsigned int foundln = 0;

    room_state.setBlockType(RoomState::bt::ATLAUNCH);
    parser::exec_until_end(room_map, room, player, rls, room_state,
            game_state, room.getATLAUNCHIns(), foundln);

    if(game_error::has_encountered_fatal()) {
        return;
    } else if(!rls.is_game_over()) {
        display(pstrings, room_map, cs_container, player, room, rls,
                room_state, game_state, same_room);
    } else {
        room_state.displayCutscenes(pstrings, cs_container);
    }
}

static void set_game_over(Player& player, game_state_s& game_state,
        PStrings const& program_strings)
{
    const std::string gameover_title = "GAME OVER";

    const std::vector<std::string> gameover_strings {
        program_strings.fetch("gameover_msg")
    };
    inventory::clear(player.inv);
    gvars::clear(player.gvars);
    /*Kinda bad implementation since it breaks the separation between code and data, but
    that'll do for now - the whole structure of the code is janky anyway*/
    game_state.next_room = "menu";
    dialogbox::display(&gameover_title, &gameover_strings, program_strings);
}

bool Room::load(PStrings const& pstrings,
        std::unordered_map<std::string, Room> const& room_map,
        CutscenesContainer const& cs_container, Player& player,
        RoomLoopState& rls, game_state_s& game_state) const
{
    bool same_room = false;
    do {
        RoomState room_state;

        game_state.next_room = m_name;

        atlaunch(pstrings, room_map, cs_container, player, *this, rls,
                room_state, game_state, same_room);
        if(game_error::has_encountered_fatal()) {
            return false;
        } else if(rls.is_game_over()) {
            set_game_over(player, game_state, pstrings);
        } else {
            same_room = true;
        }
    } while(game_state.next_room == m_name && !game_state.should_game_exit);

    return true;
}
