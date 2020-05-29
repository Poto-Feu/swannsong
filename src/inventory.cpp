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

extern "C" {
#include "perror.h"
}

#include <algorithm>
#include <vector>
#include "inventory.hpp"
#include "vars/intvar.hpp"

namespace inventory
{

    struct gitem {
        std::string name;
        unsigned int val;
    };

    static std::vector<gitem> inventory_vec;

    static auto return_it(std::string const& p_name)
    {
        return std::find_if(inventory_vec.begin(), inventory_vec.end(),
                [p_name](gitem const& citem) {
                return citem.name == p_name;
                });
    }
    //
    //Create an entry for the specified item in inventory_list
    static void add_item_to_list(std::string const& p_name, unsigned int p_val)
    {
        inventory_vec.push_back(gitem{p_name, p_val});
    }

    //Add the specified number of item to an inventory
    static void add_n_item(std::string const& p_name, unsigned int p_val)
    {
        auto it = return_it(p_name);

        if(it != inventory_vec.end()) {
            it->val += p_val;
        } else {
            std::string err_msg = "no item corresponding (" + p_name + ")";
            perror_disp(err_msg.c_str(), true);
        }
    }

    /*Add the specified number of an item - if it doesn't exist in
    inventory_vec, the function adds the item to it*/
    void player_getitem(std::string const& p_name, unsigned int val)
    {
        auto it = return_it(p_name);

        if(it != inventory_vec.cend()) {
            add_n_item(p_name, val);
        } else add_item_to_list(p_name, val);
    }

    /*Reduce the specified number of item - and remove the item from the vector
    if the result is equal to 0 or less*/
    void player_useitem(std::string const& p_name, unsigned int p_val)
    {
        auto it = return_it(p_name);

        if(it != inventory_vec.end()) {
            if(p_val < it->val) {
                it->val -= p_val;
            } else inventory_vec.erase(it);
        } else {
            std::string err_msg = "item not found in inventory (" + p_name +
                ")";
            perror_disp(err_msg.c_str(), false);
        }
    }

    //Return the number of pieces of an item present in the inventory
    unsigned int return_item_n(std::string const& p_name)
    {
        unsigned int rtrn_val = 0;
        auto it = return_it(p_name);

        if(it != inventory_vec.cend()) {
            rtrn_val = it->val;
        } else {
            std::string err_msg = "inventory item not found (" + p_name + ")";
            perror_disp(err_msg.c_str(), true);
        }
        return rtrn_val;
    }
}
