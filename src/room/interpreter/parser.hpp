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

#include "room/room_struct.hpp"

namespace parser
{
    void skip_until_end(std::vector<TokenVec> const& block_vector, unsigned int& blockln);
    void exec_until_end(std::vector<TokenVec> const& block_vector, room_struct& p_struct,
            unsigned int& i);
    bool splitline(std::string& type, std::string& arg, std::string ins);
}
