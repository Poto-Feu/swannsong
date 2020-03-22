/*
    Copyright (C) 2020 Adrien Saad

    This file is part of SwannSong.

    SwannSong is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    SwannSong is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "inventory.h"

/*Not named item in order to prevent conflict with some libs*/
typedef struct
{
    char* name;
    int n;
} gitem;

static gitem* inventory_list = NULL;
static int inv_ln = 0;

static bool check_gitem_exist(char* pname, int* ind);
static void inventory_additem_tolist(char* pname, int val);

/*Add the specified number of an item - if it doesn't exist in inventory_list,
the function adds the item to it*/
void inventory_player_getitem(char* name, int val)
{
    int i = -1;

    if(check_gitem_exist(name, &i))
    {
        if(inventory_list[i].n + val < 0)
        {
            inventory_list[i].n = 0;
        } else
        {
            inventory_list[i].n += val;
        }
    } else
    {
        inventory_additem_tolist(name, val);
    }
}

/*Return the number of pieces of an item present in the inventory*/
int inventory_return_item_n(char* pname)
{
    int ind = -1;

    if(check_gitem_exist(pname, &ind))
    {
        return inventory_list[ind].n;
    } else
    {
        return 0;
    }
}

/*Create an entry for the specified item in inventory_list*/
static void inventory_additem_tolist(char* pname, int val)
{
    if(inv_ln == 0)
    {
        inventory_list = calloc(1, sizeof(gitem));
        inventory_list[0].name = calloc(strlen(pname)+1, sizeof(char));
        strcpy(inventory_list[0].name, pname);
        inventory_list[0].n = val;
        inv_ln++;
    } else
    {
        gitem* temp_arr = calloc(inv_ln, sizeof(gitem));

        for(int i = 0; i < inv_ln; i++)
        {
            int str_ln = strlen(inventory_list[i].name);

            temp_arr[i].name = calloc(str_ln+1, sizeof(char));
            strcpy(temp_arr[i].name, inventory_list[i].name);
            temp_arr[i].n = inventory_list[i].n;
            free(inventory_list[i].name);
        }
        free(inventory_list);

        inventory_list = calloc(inv_ln+1, sizeof(gitem));
        for(int i = 0; i < inv_ln; i++)
        {
            int str_ln = strlen(temp_arr[i].name);

            inventory_list[i].name = calloc(str_ln+1, sizeof(char));
            strcpy(inventory_list[i].name, temp_arr[i].name);
            inventory_list[i].n = temp_arr[i].n;
            free(temp_arr[i].name);
        }
        free(temp_arr);

        inventory_list[inv_ln].name = calloc(strlen(pname)+1, sizeof(char));
        strcpy(inventory_list[inv_ln].name, pname);
        inventory_list[inv_ln].n = val;
        inv_ln++;
    }
}

static bool check_gitem_exist(char* pname, int* ind)
{
    bool isfound = false;

    for(int i = 0; i < inv_ln && !isfound; i++)
    {
        if(!strcmp(pname, inventory_list[i].name))
        {
            *ind = i;
            isfound = true;
        }
    }

    return isfound;
}
