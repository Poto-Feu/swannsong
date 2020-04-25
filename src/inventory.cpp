/*
    Copyright (C) 2020 Adrien Saad

    This file is part of SwannSong.

    SwannSong is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SwannSong is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong.  If not, see <https://www.gnu.org/licenses/>.
*/

extern "C" {
#include "perror.h"
}

#include <vector>
#include "inventory.h"
#include "vars/intvar.h"

/*Not named item in order to prevent conflict with some libs*/
typedef intvar gitem;

static std::vector<gitem> inventory_arr;

static void inventory_additem_tolist(char* pname, int val);
static void inventory_add_n_item(uint16_t p_ind, int val);

/*Add the specified number of an item - if it doesn't exist in inventory_list,
the function adds the item to it*/
void inventory_player_getitem(char* p_name, int val)
{
    uint16_t p_ind = 0;

    if(intvar_search_ind(&p_ind, p_name, &inventory_arr))
    {
        inventory_add_n_item(p_ind, val);
    } else
    {
        inventory_additem_tolist(p_name, val);
    }
}

/*Return the number of pieces of an item present in the inventory*/
int inventory_return_item_n(char* p_name)
{
    int r_val = -1;
    uint16_t p_ind = 0;

    if(intvar_search_ind(&p_ind, p_name, &inventory_arr))
    {
        intvar_return_value(&r_val, p_ind, &inventory_arr);
    } else perror_disp("INV_VAR_NF", 1);

    return r_val;
}

/*Create an entry for the specified item in inventory_list*/
static void inventory_additem_tolist(char* p_name, int p_val)
{
    intvar elem = INIT_INTVAR(static_cast<std::string>(p_name), p_val);

    intvar_add_var_to_arr(&inventory_arr, elem);
}

/*Add the specified number of item to an inventory*/
static void inventory_add_n_item(uint16_t p_ind, int p_val)
{
    int r_val = -1;

    intvar_return_value(&r_val, p_ind, &inventory_arr);
    p_val += r_val;
    intvar_set_value(r_val, p_ind, &inventory_arr);
}
