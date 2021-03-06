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

#ifndef GVARS_H
#define GVARS_H

#include <string>
#include "vars/intvar.hpp"

namespace gvars
{
    typedef intvar gvar;
    typedef intvar_type gvar_type;
    typedef std::vector<gvar> gvarVector;

    void set_var(std::string const& p_name, gvar_type p_val);
    gvar_type return_value(std::string const& p_name);
    void change_val(std::string const& p_name, gvar_type p_val);
    bool exist(std::string const& p_name);
    void replace_vector(gvarVector const& p_vec);

    //Return a vector containing all script defined game variable
    std::vector<gvar> get_gvars_vector();
}
#endif
