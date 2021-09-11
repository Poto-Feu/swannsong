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

#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <string>
#include <vector>

#include "pstrings.hpp"

namespace inventory
{
    typedef uint32_t item_val_type;

    struct gitem {
        gitem(std::string const& p_name, item_val_type p_val) : name(p_name), val(p_val) { }
        gitem() { }

        std::string name;
        item_val_type val;
    };

    typedef std::vector<gitem> Inventory;

    /*Add the specified number of an item - if it doesn't exist in inventory_vec, the function adds 
    the item to it*/
    void getitem(Inventory& p_inv, std::string const& p_name, item_val_type val);
    /*Reduce the specified number of item - and remove the item from the vector if the result is 
    equal to 0 or less*/
    void useitem(Inventory& p_inv, std::string const& p_name, item_val_type val);
    /*Return the number of pieces of an item present in the inventory - returns 0 if the item
    does not exist*/
    item_val_type return_item_n(Inventory const& p_inv, std::string const& p_name);
    //Remove all items from the inventory
    void clear(Inventory& p_inv);
}
#endif
