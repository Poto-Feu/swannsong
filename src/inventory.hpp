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
#include <vector>

namespace inventory
{
    typedef uint32_t item_val_type;

    struct gitem {
        gitem(std::string const& p_name, item_val_type p_val) : name(p_name), val(p_val) { }
        gitem() { }

        std::string name;
        item_val_type val;
    };

    typedef std::vector<gitem> gitemVector;

    void player_getitem(std::string const& p_name, item_val_type val);
    void player_useitem(std::string const& p_name, item_val_type val);
    item_val_type return_item_n(std::string const& p_name);
    void display_screen();
    void replace_vector(gitemVector p_vector);
    gitemVector get_inventory_vector();
}

#endif
