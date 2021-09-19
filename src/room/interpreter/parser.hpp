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


#ifndef ROOM_PARSER_HPP
#define ROOM_PARSER_HPP

#include "room/RoomClass.hpp"

class RoomDisplay;

namespace parser
{
    enum class block_type {
        ATLAUNCH,
        CHOICE
    };

    bool exec_until_end(std::vector<TokenVec> const& block_vector,
            std::unordered_map<std::string, Room> const& room_map,
            Room const& room, parser::block_type block_type,
            Player& player, RoomLoopState& rls, RoomDisplay* room_display,
            game_state_s& game_state,
            std::vector<std::string>& cutscenes_vec, unsigned int& i);
    bool splitline(std::string& type, std::string& arg, std::string ins);
}
#endif
