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
#include <stdint.h>
#include <stdbool.h>
#include "vars/intvar.h"
#include "perror.h"

static void add_first_elem(intvar_arr* p_arr, intvar p_var);
static void add_elem_to_arr(intvar_arr* p_arr, intvar p_var);

/*Add an intvar into an intvar_arr*/
void intvar_add_var_to_arr(intvar_arr* p_arr, intvar p_var)
{
    if (p_arr->ln == 0)
    {
        add_first_elem(p_arr, p_var);
        p_arr->ln = 1;
    } else
    {
        add_elem_to_arr(p_arr, p_var);
        p_arr->ln = p_arr->ln + 1;
    }
}

/*Fetch the index of a specified intvar - returns false if not found*/
bool intvar_search_ind(int* p_ind, char* p_name, intvar_arr* p_arr)
{
    bool isfnd = false;
    *p_ind = -1;
    uint16_t arr_ln = p_arr->ln;

    for(int i = 0; i < arr_ln && !isfnd; i++)
    {
        if(!strcmp(p_arr->list[i].name, p_name))
        {
            *p_ind = i;
            isfnd = true;
        }
    }

    return isfnd;
}

/*Return the value of the intvar on the specified index*/
void intvar_return_value(int* r_val, int p_ind, intvar_arr* p_arr)
{
    if(p_ind >= p_arr->ln)
    {
        perror_disp("OOR_ARR_INDEX", 1);
    } else
    {
        *r_val = p_arr->list[p_ind].val;
    }
}

static void add_first_elem(intvar_arr* p_arr, intvar p_var)
{
    int str_ln = strlen(p_var.name);
    
    p_arr->list = calloc(1, sizeof(intvar));
    p_arr->list[0].name = calloc(str_ln+1, sizeof(char));

    strcpy(p_arr->list[0].name, p_var.name);
    p_arr->list[0].val = p_var.val;
}

static void add_list_row(intvar_arr* p_arr);

static void add_elem_to_arr(intvar_arr* p_arr, intvar p_var)
{
    int str_ln = strlen(p_var.name);
    uint16_t arr_ln = p_arr->ln;

    add_list_row(p_arr);
    p_arr->list[arr_ln].name = calloc(str_ln+1, sizeof(char));
    strcpy(p_arr->list[arr_ln].name, p_var.name);
    p_arr->list[arr_ln].val = p_var.val;
}

static void create_temp_arr(intvar** temp_arr, intvar_arr* p_arr);
static void temp_arr_cpy_back(intvar_arr** p_arr, intvar** temp_arr);

/*Expand an array to hold one more element*/
static void add_list_row(intvar_arr* p_arr)
{
    intvar* temp_arr = NULL;

    create_temp_arr(&temp_arr, p_arr);
    temp_arr_cpy_back(&p_arr, &temp_arr);
}

static void create_temp_arr(intvar** temp_arr, intvar_arr* p_arr)
{
    uint16_t arr_ln = p_arr->ln;

    *temp_arr = calloc(arr_ln+1, sizeof(intvar));

    for(int i = 0; i < arr_ln; i++)
    {
        int str_ln = strlen(p_arr->list[i].name);

        (*temp_arr)[i].name = calloc(str_ln+1, sizeof(char));
        strcpy((*temp_arr)[i].name, p_arr->list[i].name);
        (*temp_arr)[i].val = p_arr->list[i].val;
        free(p_arr->list[i].name);
    }
    free(p_arr->list);
}

static void temp_arr_cpy_back(intvar_arr** p_arr, intvar** temp_arr)
{
    uint16_t arr_ln = (*p_arr)->ln;

    (*p_arr)->list = calloc(arr_ln+1, sizeof(intvar));
    
    for(int i = 0; i < arr_ln; i++)
    {
        int str_ln = strlen((*temp_arr)[i].name);

        (*p_arr)->list[i].name = calloc(str_ln+1, sizeof(char));
        strcpy((*p_arr)->list[i].name, (*temp_arr)[i].name);
        (*p_arr)->list[i].val = (*temp_arr)[i].val;
        free((*temp_arr)[i].name);
    }
    free(*temp_arr);
}
