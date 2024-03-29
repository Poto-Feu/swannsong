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

#ifndef INTVAR_H
#define INTVAR_H

#include <string>
#include <vector>

typedef int32_t intvar_type;

struct intvar
{
    intvar(std::string const& p_name, intvar_type p_val) : name(p_name), val(p_val) { }
    intvar() { }

    std::string name;
    intvar_type val;
};

namespace intvarm
{

    void add_var_to_arr(std::vector<intvar>& p_vec, intvar& p_var);
    intvar_type return_value(std::string const& p_name, std::vector<intvar> const& p_vec);
    void set_value(intvar_type p_val, std::string const& p_name, std::vector<intvar>& p_vec);
}

#endif
