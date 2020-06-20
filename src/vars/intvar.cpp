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
#include "vars/intvar.hpp"
#include "game_error.hpp"

namespace intvarm
{
    //Return a constant iterator
    auto return_it(std::string const& p_name, std::vector<intvar>& p_vec)
    {
        return std::find_if(p_vec.begin(), p_vec.end(), [p_name](intvar const& cvar) {
                return cvar.name == p_name;
                });
    }

    //Add an intvar into an std::vector<intvar>
    void add_var_to_arr(std::vector<intvar>& p_vec, intvar& p_var)
    {
        p_vec.push_back(p_var);
    }

    //Return the value of the intvar on the specified index
    int16_t return_value(std::string const& p_name, std::vector<intvar>& p_vec)
    {
        int rtrn_val = -1;
        auto it = return_it(p_name, p_vec);

        if(it != p_vec.end()) {
            rtrn_val = it->val;
        }

        return rtrn_val;
    }

    //Set the value of the specified intvar
    void set_value(int16_t p_val, std::string const& p_name, std::vector<intvar>& p_vec)
    {
        auto it = return_it(p_name, p_vec);

        if(it != p_vec.end()) it->val = p_val;
        else game_error::fatal_error("game var does not exists (" + p_name + ")");
    }
}
