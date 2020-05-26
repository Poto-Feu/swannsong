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

extern "C"
{
#include "perror.h"
}

#include <string>
#include <vector>
#include "gvars.hpp"
#include "intvar.hpp"

typedef intvar gvar;

static std::vector<intvar> gvar_vec;

static void add_to_list(std::string const p_name, int p_val)
{
    gvar elem(p_name, p_val);

    intvarm::add_var_to_arr(gvar_vec, elem);
}

namespace gvars
{
    void set_var(std::string const p_name, int p_val)
    {
        int p_ind = 0;

        if(intvarm::search_ind(p_ind, p_name, gvar_vec))
        {
            perror_disp("gvar already exists", true);
        } else add_to_list(p_name, p_val);
    }

    int return_value(std::string const p_name)
    {
        int r_val = -1;
        int p_ind = 0;

        if(intvarm::search_ind(p_ind, p_name, gvar_vec))
        {
            r_val = intvarm::return_value(p_ind, gvar_vec);
        } else perror_disp("gvar does not exist", true);

        return r_val;
    }

    void change_val(std::string const p_name, int p_val)
    {
        int p_ind = 0;

        if(intvarm::search_ind(p_ind, p_name, gvar_vec))
        {
            intvarm::set_value(p_val, p_ind, gvar_vec);
        } else perror_disp("gvar does not exist", true);
    }

    bool exist(std::string const p_name)
    {
        int p_ind = 0;

        if(intvarm::search_ind(p_ind, p_name, gvar_vec)) return true;
        return false;
    }
}
