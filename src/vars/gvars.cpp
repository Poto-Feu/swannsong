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
#include "vars/intvar.hpp"
#include "game_error.hpp"

namespace gvars
{
    typedef intvar gvar;

    static std::vector<intvar> gvar_vec;

    static void add_to_list(std::string const& p_name, int16_t p_val)
    {
        gvar elem(p_name, p_val);
        intvarm::add_var_to_arr(gvar_vec, elem);
    }

    static auto check_exist(std::string const& p_name)
    {
        return std::find_if(gvar_vec.cbegin(), gvar_vec.cend(),
                [&p_name](gvar const& cvar) {
                return cvar.name == p_name;
                }) != gvar_vec.cend();
    }

    void set_var(std::string const& p_name, int16_t p_val)
    {
        if(check_exist(p_name)) game_error::fatal_error("gvar already exists (" + p_name + ")");
        else add_to_list(p_name, p_val);
    }

    int16_t return_value(std::string const& p_name)
    {
        int16_t r_val = -1;

        if(check_exist(p_name)) r_val = intvarm::return_value(p_name, gvar_vec);
        else game_error::fatal_error("gvar does not exist");

        return r_val;
    }

    void change_val(std::string const& p_name, int16_t p_val)
    {
        if(check_exist(p_name)) intvarm::set_value(p_val, p_name, gvar_vec);
        else game_error::fatal_error("gvar does not exist");
    }

    bool exist(std::string const& p_name)
    {
        if(check_exist(p_name)) return true;
        else return false;
    }
}
