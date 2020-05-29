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

#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>

namespace inventory
{
    void player_getitem(std::string const& p_name, unsigned int val);
    void player_useitem(std::string const& p_name, unsigned int val);
    unsigned int return_item_n(std::string const& p_name);
}

#endif
