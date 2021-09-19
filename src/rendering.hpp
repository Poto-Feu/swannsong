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

#ifndef RENDERING_HPP
#define RENDERING_HPP

#include "player/inventory.hpp"

struct Cutscene;
struct RoomDisplay;
class PStrings;
class Room;

namespace rendering {
    void display_cutscene(PStrings const& pstrings, Cutscene const& cutscene);
    void display_inventory(inventory::Inventory const& inv,
            PStrings const& pstrings);
    std::string display_room(PStrings const& pstrings, Room const& room,
            RoomDisplay const& room_display,
            const std::string *error_msg = nullptr);
}
#endif
