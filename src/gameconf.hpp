/*
    Copyright (C) 2021 Adrien Saad

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
    along with SwannSong Adventure.  If not, see
    <https://www.gnu.org/licenses/>.
*/

#ifndef GAMECONF_HPP
#define GAMECONF_HPP

#include <memory>
#include <string>
#include <vector>

namespace gameconf
{
    struct gcvars;
    typedef std::shared_ptr<gcvars> gcvars_ptr;

    gcvars_ptr readfile(std::string const& data_path);
    std::vector<std::string> const* get_var(gameconf::gcvars_ptr const& vars,
            std::string const& var_name);
}
#endif
