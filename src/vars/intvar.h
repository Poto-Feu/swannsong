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

#ifndef INTVAR_H
#define INTVAR_H

#include <stdint.h>
#include <string>
#include <vector>

struct intvar
{
    std::string name;
    int val;
};

#define INIT_INTVAR(n, v) {n, v}

void intvar_add_var_to_arr(std::vector<intvar>* p_arr, intvar p_var);
bool intvar_search_ind(uint16_t* p_ind, char* p_name,
        std::vector<intvar>* p_arr);
void intvar_return_value(int* r_val, uint16_t p_ind,
        std::vector<intvar>* p_arr);
void intvar_set_value(int r_val, uint16_t p_ind, std::vector<intvar>* p_arr);

#endif
