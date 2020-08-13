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

#include <unordered_map>
#include "room.hpp"
#include "find.hpp"
#include "RoomManager.hpp"
#include "pcurses.hpp"
#include "pstrings.h"

namespace roommod
{
    static void unfinished_game()
    {
        display_server::clear_screen();
        pcurses::display_center_string(pstrings::fetch("unfinished_str"), pcurses::top_margin);
    }

    //Start the game loop which loads rooms until the end signal is enabled
    void start_loop(std::string const& id)
    {
        std::string curr_room_id = id;
        RoomManager rmm;

        while(!rmm.is_endgame() && !rmm.is_unfinished()) {
            static std::unordered_map<std::string, Room> room_map;
            bool room_fnd = false;
            Room currentRoom;

            auto room_it = room_map.find(curr_room_id.c_str());

            if(room_it != room_map.cend()) {
                room_fnd = true;
                currentRoom = room_it->second;
            } else {
                currentRoom = Room(curr_room_id);
                currentRoom.setRoomLine(room_find::roomline(curr_room_id));
            }

            if(!currentRoom.load(rmm)) break;
            else if(!room_fnd) room_map.insert({currentRoom.getName(), std::move(currentRoom)});

            if(!rmm.is_endgame() && !rmm.is_unfinished()) curr_room_id = rmm.getNextRoom();
        }
        display_server::clear_screen();
        if(rmm.is_unfinished()) unfinished_game();
    }
}
