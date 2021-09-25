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

#ifndef ROOMS_HPP
#define ROOMS_HPP

#include <memory>

#include "pstrings.hpp"

class Room;

namespace rooms {
    struct RoomsData;
    typedef std::shared_ptr<RoomsData> RoomsData_ptr;

    RoomsData_ptr init_data(pstrings::ps_data_ptr const& pstrings_data,
            std::string const& data_path);
    Room const* get_room(RoomsData_ptr const& rooms_data,
            std::string const& name);
}
#endif
