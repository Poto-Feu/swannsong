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

#ifndef ROOM_MANAGER_HPP
#define ROOM_MANAGER_HPP

#include <unordered_map>
#include <string>

#include "room/RoomClass.hpp"
#include "room/RoomLoopState.hpp"
#include "room/RoomState.hpp"

class RoomManager
{
    public:

        //Start the game loop which loads rooms until the end signal is enabled
        void startLoop(std::string const& start_room);

    private:

        std::unordered_map<std::string, Room> m_room_map;
        RoomLoopState m_rls;
};

#endif
