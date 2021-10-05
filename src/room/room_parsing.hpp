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

#ifndef ROOM_PARSING_HPP
#define ROOM_PARSING_HPP

#include <unordered_map>

#include "pstrings.hpp"

class Room;

namespace room_parsing {
    bool parse_rooms_file(pstrings::ps_data_ptr const& pstrings_data,
            std::string const& data_path,
            std::unordered_map<std::string, Room>& room_map);
}
#endif
