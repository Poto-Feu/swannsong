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
#include "vars/gvars.hpp"
#include "game_error.hpp"

namespace gvars
{
    static void add_to_list(gvarVector& p_vec, std::string const& p_name, gvar_type p_val)
    {
        gvar elem(p_name, p_val);
        intvarm::add_var_to_arr(p_vec, elem);
    }

    bool set_var(gvarVector& p_vec, std::string const& p_name, gvar_type p_val)
    {
        if(!exist(p_vec, p_name)) {
            add_to_list(p_vec, p_name, p_val);
            return true;
        } else {
            change_val(p_vec, p_name, p_val);
            return false;
        }
    }

    gvar_type return_value(gvarVector const& p_vec, std::string const& p_name)
    {
        if(!exist(p_vec, p_name)) {
            game_error::fatal_error("gvar does not exist");
            return -1;
        } else return intvarm::return_value(p_name, p_vec);
    }

    bool change_val(gvarVector& p_vec, std::string const& p_name, gvar_type p_val)
    {
        if(!exist(p_vec, p_name)) {
            game_error::fatal_error("gvar does not exist");
            return false;
        } else {
            intvarm::set_value(p_val, p_name, p_vec);
            return true;
        }
    }

    bool exist(gvarVector const& p_vec, std::string const& p_name)
    {
        return std::find_if(p_vec.cbegin(), p_vec.cend(),
                [&p_name](gvar const& cvar) {
                return cvar.name == p_name;
                }) != p_vec.cend();
    }

    void clear(gvarVector& p_vec)
    {
        p_vec.clear();
    }

    void replace_vector(gvarVector& target, gvarVector const& source)
    {
        target = source;
    }
}
