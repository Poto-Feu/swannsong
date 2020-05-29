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

#include <cstdint>
#include <vector>
#include "vars/intvar.hpp"

namespace intvarm
{
    //Add an intvar into an std::vector<intvar>
    void add_var_to_arr(std::vector<intvar>& p_vec, intvar p_var)
    {
        p_vec.push_back(p_var);
    }

    //Fetch the index of the specified intvar - returns false if not found
    bool search_ind(int& p_ind, std::string p_name,
            std::vector<intvar> const& p_vec)
    {
        int i = 0;
        bool isfnd = false;
        std::string str_name(p_name);

        p_ind = -1;

        for(auto const& x : p_vec) {
            if(x.name == str_name) {
                p_ind = i;
                isfnd = true;
                break;
            }
        }

        return isfnd;
    }

    //Return the value of the intvar on the specified index
    int return_value(int p_ind, std::vector<intvar>& p_vec)
    {
        if(p_ind >= static_cast<int>(p_vec.size())) {
            perror_disp("OOR_ARR_INDEX", true);
            return -1;
        } else return p_vec[p_ind].val;
       
    }

    //Set the value of the specified intvar
    void set_value(int p_val, int p_ind, std::vector<intvar>& p_vec)
    {
        p_vec[p_ind].val = p_val;
    }
}
