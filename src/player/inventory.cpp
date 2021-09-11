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

#include <algorithm>

#include "player/inventory.hpp"
#include "dialogbox.hpp"
#include "game_error.hpp"
#include "pcurses.hpp"
#include "userio.hpp"

namespace inventory
{
    static auto return_it(Inventory& p_inv, std::string const& p_name)
    {
        return std::find_if(p_inv.begin(), p_inv.end(),
                [&p_name](gitem const& citem) {
                return citem.name == p_name;
                });
    }

    static auto return_const_it(Inventory const& p_inv, std::string const& p_name)
    {
        return std::find_if(p_inv.cbegin(), p_inv.cend(),
                [&p_name](gitem const& citem) {
                return citem.name == p_name;
                });
    }

    //Create an entry for the specified item in inventory_list
    static void add_item_to_list(Inventory& p_inv, std::string const& p_name, item_val_type p_val)
    {
        p_inv.push_back(gitem{p_name, p_val});
    }

    //Add the specified number of item to an inventory
    static void add_n_item(Inventory& p_inv, std::string const& p_name, item_val_type p_val)
    {
        auto it = return_it(p_inv, p_name);

        if(it != p_inv.end()) it->val += p_val;
        else game_error::fatal_error("no item corresponding (" + p_name + ")");
    }

    void getitem(Inventory& p_inv, std::string const& p_name, item_val_type val)
    {
        auto it = return_it(p_inv, p_name);

        if(it != p_inv.cend()) add_n_item(p_inv, p_name, val);
        else add_item_to_list(p_inv, p_name, val);
    }

    void useitem(Inventory& p_inv, std::string const& p_name, item_val_type p_val)
    {
        auto it = return_it(p_inv, p_name);

        if(it != p_inv.end()) {
            if(p_val < it->val) it->val -= p_val;
            else p_inv.erase(it);
        } else game_error::emit_warning("item not found in inventory (" + p_name + ")");
    }

    item_val_type return_item_n(Inventory const& p_inv, std::string const& p_name)
    {
        item_val_type rtrn_val = 0;
        auto it = return_const_it(p_inv, p_name);

        if(it != p_inv.cend()) rtrn_val = it->val;
        else rtrn_val = 0;

        return rtrn_val;
    }

    void clear(Inventory& p_inv)
    {
        p_inv.clear();
    }
}
