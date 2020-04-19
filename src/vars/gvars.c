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

#include <stdlib.h>
#include <stdint.h>
#include "vars/gvars.h"
#include "perror.h"
#include "vars/intvar.h"

typedef intvar gvar;
typedef intvar_arr gvar_a;

static gvar_a gvar_arr = INIT_INTVAR_ARR;

static void gvars_add_to_list(char* p_name, int p_val);

void gvars_set_var(char* p_name, int p_val)
{
    uint16_t p_ind = 0;

    if(intvar_search_ind(&p_ind, p_name, &gvar_arr))
    {
        perror_disp("gvar already exists", 1);
    } else
    {
        gvars_add_to_list(p_name, p_val);
    }
}

int gvars_return_value(char* p_name)
{
    int r_val = -1;
    uint16_t p_ind = 0;

    if(intvar_search_ind(&p_ind, p_name, & gvar_arr))
    {
        intvar_return_value(&r_val, p_ind, &gvar_arr);
    } else
    {
        perror_disp("gvar does not exist", 1);
    }

    return r_val;
}

void gvars_change_val(char* p_name, int p_val)
{
    uint16_t p_ind = 0;

    if(intvar_search_ind(&p_ind, p_name, &gvar_arr))
    {
        intvar_set_value(p_val, p_ind, &gvar_arr);
    } else
    {
        perror_disp("gvar does not exist", 1);
    }
}

bool gvars_exist(char* p_name)
{
    uint16_t p_ind = 0;

    if(intvar_search_ind(&p_ind, p_name, &gvar_arr))
    {
        return true;
    }
    return false;
}

static void gvars_add_to_list(char* p_name, int p_val)
{
    gvar elem = INIT_INTVAR(p_name, p_val);

    intvar_add_var_to_arr(&gvar_arr, elem);
}
