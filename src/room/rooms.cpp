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
    along with SwannSong Adventure. If not, see
    <https://www.gnu.org/licenses/>.
*/

#include <algorithm>

#include "room/rooms.hpp"
#include "room/RoomClass.hpp"
#include "room/room_parsing.hpp"

struct rooms::RoomsData {
    std::unordered_map<std::string, Room> rooms_map;
};

rooms::RoomsData_ptr rooms::init_data(
        pstrings::ps_data_ptr const& pstrings_data,
        std::string const& data_path)
{
    RoomsData_ptr room_data(new RoomsData);

    if(!room_parsing::parse_rooms_file(pstrings_data, data_path,
                room_data->rooms_map)) {
        return nullptr;
    }

    return room_data;
}

Room const* rooms::get_room(RoomsData_ptr const& rooms_data,
        std::string const& name)
{
    auto const& room_it = rooms_data->rooms_map.find(name);

    if(room_it == rooms_data->rooms_map.cend()) {
        return nullptr;
    } else {
        return &room_it->second;
    }
}
