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

#include "room/RoomManager.hpp"
#include "room/find.hpp"
#include "display_server.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "userio.h"

static void unfinished_game()
{
    const display_server::coord_struct exit_struct {pcurses::lines - 3, pcurses::margin};

    display_server::clear_screen();
    pcurses::display_center_string(pstrings::fetch("unfinished_str"), pcurses::top_margin);
    display_server::add_string(pstrings::fetch("exit_penter"), exit_struct, A_BOLD);
    display_server::show_screen();
    userio::waitenter();
}

void RoomManager::startLoop(std::string const& start_room)
{
    std::string curr_room_id = start_room;

    while(!m_rls.is_endgame() && !m_rls.is_unfinished()) {
        bool room_fnd = false;
        Room currentRoom;

        auto room_it = m_room_map.find(curr_room_id.c_str());

        if(room_it != m_room_map.cend()) {
            room_fnd = true;
            currentRoom = room_it->second;
        } else {
            currentRoom = Room(curr_room_id);
            currentRoom.setRoomLine(room_find::roomline(curr_room_id));
        }

        if(!currentRoom.load(m_rls, m_player)) break;
        else if(!room_fnd) m_room_map.insert({currentRoom.getName(), std::move(currentRoom)});

        if(!m_rls.is_endgame() && !m_rls.is_unfinished()) curr_room_id = m_rls.getNextRoom();
    }
    display_server::clear_screen();
    if(m_rls.is_unfinished()) unfinished_game();
}
