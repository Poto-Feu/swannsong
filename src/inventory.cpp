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

#include <vector>
#include "inventory.hpp"
#include "vars/intvar.hpp"

namespace inventory
{
    //Not named item in order to prevent conflict with some libs
    typedef intvar gitem;

    static std::vector<gitem> inventory_vec;

    //Create an entry for the specified item in inventory_list
    static void additem_tolist(std::string const& p_name, int p_val)
    {
        intvar elem(p_name, p_val);

        intvarm::add_var_to_arr(inventory_vec, elem);
    }

    //Add the specified number of item to an inventory
    static void add_n_item(int p_ind, unsigned int p_val)
    {
        int r_val = intvarm::return_value(p_ind, inventory_vec);

        r_val += p_val;
        intvarm::set_value(r_val, p_ind, inventory_vec);
    }

    /*Add the specified number of an item - if it doesn't exist in
    inventory_vec, the function adds the item to it*/
    void player_getitem(std::string const& p_name, unsigned int val)
    {
        int p_ind = 0;

        if(intvarm::search_ind(p_ind, p_name, inventory_vec))
        {
            add_n_item(p_ind, val);
        } else additem_tolist(p_name, val);
    }

    //Return the number of pieces of an item present in the inventory
    unsigned int return_item_n(std::string const& p_name)
    {
        int r_val = -1;
        int p_ind = 0;

        if(intvarm::search_ind(p_ind, p_name, inventory_vec)) {
            r_val = intvarm::return_value(p_ind, inventory_vec);
        } else {
            std::string err_msg = "Inventory item not found (" + p_name + ")";
            perror_disp(err_msg.c_str(), true);
        }
        return r_val;
    }
}
