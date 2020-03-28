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

#ifndef INTVAR_H
#define INTVAR_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    char* name;
    int val;
} intvar;

typedef struct
{
    uint16_t ln;
    intvar* list;
} intvar_arr;

#define INIT_INTVAR_ARR {.ln = 0, .list = NULL}
#define INIT_INTVAR(n, v) {.name = n, .val = v}

void intvar_add_var_to_arr(intvar_arr* p_arr, intvar p_var);
bool intvar_search_ind(int* p_ind, char* p_name, intvar_arr* p_arr);

#endif
