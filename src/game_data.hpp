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

#ifndef GAME_DATA_HPP
#define GAME_DATA_HPP

#include "cutsecenes.hpp"
#include "pstrings.hpp"
#include "room/rooms.hpp"

struct GameData {
    cutscenes::csdata_ptr cs_data;
    pstrings::ps_data_ptr pstrings_data;
    rooms::RoomsData_ptr rooms_data;
};

bool init_game_data(std::string const& game_data_path,
        std::string const& lang_code, GameData& game_data);
#endif
