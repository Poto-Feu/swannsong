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

#include <algorithm>
#include "inventory.hpp"
#include "game_error.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "userio.h"

namespace inventory
{
    struct gitem {
        std::string name;
        uint16_t val;
    };

    static std::vector<gitem> inventory_vec;

    static auto return_it(std::string const& p_name)
    {
        return std::find_if(inventory_vec.begin(), inventory_vec.end(),
                [&p_name](gitem const& citem) {
                return citem.name == p_name;
                });
    }

    //Create an entry for the specified item in inventory_list
    static void add_item_to_list(std::string const& p_name, uint16_t p_val)
    {
        inventory_vec.push_back(gitem{p_name, p_val});
    }

    //Add the specified number of item to an inventory
    static void add_n_item(std::string const& p_name, uint16_t p_val)
    {
        auto it = return_it(p_name);

        if(it != inventory_vec.end()) it->val += p_val;
        else game_error::fatal_error("no item corresponding (" + p_name + ")");
    }

    /*Add the specified number of an item - if it doesn't exist in inventory_vec, the function adds 
    the item to it*/
    void player_getitem(std::string const& p_name, uint16_t val)
    {
        auto it = return_it(p_name);

        if(it != inventory_vec.cend()) add_n_item(p_name, val);
        else add_item_to_list(p_name, val);
    }

    /*Reduce the specified number of item - and remove the item from the vector if the result is 
    equal to 0 or less*/
    void player_useitem(std::string const& p_name, uint16_t p_val)
    {
        auto it = return_it(p_name);

        if(it != inventory_vec.end()) {
            if(p_val < it->val) it->val -= p_val;
            else inventory_vec.erase(it);
        } else game_error::emit_warning("item not found in inventory (" + p_name + ")");
    }

    //Return the number of pieces of an item present in the inventory
    uint16_t return_item_n(std::string const& p_name)
    {
        unsigned int rtrn_val = 0;
        auto it = return_it(p_name);

        if(it != inventory_vec.cend()) rtrn_val = it->val;
        else rtrn_val = 0;

        return rtrn_val;
    }

    //Display the inventory screen
    void display_screen()
    {
        display_server::clear_screen();
        move(pcurses::top_margin, pcurses::margin);

        for(auto const& it : inventory_vec) {
            int str_line = pcurses::top_margin;
            std::string disp_str = it.name;
            std::string str_name = "item_" + it.name;

            if(pstrings::check_exist(str_name)) disp_str = pstrings::fetch(str_name);

            disp_str += "   ";
            disp_str += std::to_string(return_item_n(it.name));
            pcurses::display_center_string(disp_str, str_line);
            ++str_line;

            if(str_line >= LINES - 6) {
                if(pstrings::check_exist("inventory_more")) {
                    pcurses::display_center_string(pstrings::fetch("inventory_more"), str_line);
                } else pcurses::display_center_string("(And more...)");
                break;
            }
        }

        display_server::add_string(pstrings::fetch("continue_penter"),
                {pcurses::lines - 3, pcurses::margin}, A_BOLD);
        display_server::show_screen();
        userio::waitenter();
    }
}
