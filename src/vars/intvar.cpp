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

#include <cstdint>
#include <vector>
#include "vars/intvar.h"

/*Add an intvar into an std::vector<intvar>*/
void intvar_add_var_to_arr(std::vector<intvar>* p_arr, intvar p_var)
{
    (*p_arr).push_back(p_var);
}

/*Fetch the index of the specified intvar - returns false if not found*/
bool intvar_search_ind(uint16_t* p_ind, char* p_name, std::vector<intvar>* p_arr)
{
    int i = 0;
    bool isfnd = false;
    std::string str_name(p_name);

    *p_ind = -1;

    for(const auto& x : (*p_arr))
    {
        if(x.name == str_name)
        {
            *p_ind = i;
            isfnd = true;
            break;
        }
    }

    return isfnd;
}

/*Return the value of the intvar on the specified index*/
void intvar_return_value(int* r_val, uint16_t p_ind, std::vector<intvar>* p_arr)
{
    if(p_ind >= (int)(*p_arr).size()) perror_disp("OOR_ARR_INDEX", true);
    else *r_val = (*p_arr)[p_ind].val;
}

/*Set the value of the specified intvar*/
void intvar_set_value(int p_val, uint16_t p_ind, std::vector<intvar>* p_arr)
{
    (*p_arr)[p_ind].val = p_val;
}
