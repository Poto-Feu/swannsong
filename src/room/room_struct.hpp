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

#pragma once

#include <unordered_map>

#include "room/RoomClass.hpp"
#include "room/RoomState.hpp"

struct room_struct
{
    Room const& currRoom;
    RoomState currState;
    RoomLoopState& currLoopState;
    Player& currPlayer;
    std::unordered_map<std::string, Room> const& roomMap;
    PStrings const& program_strings;
};
