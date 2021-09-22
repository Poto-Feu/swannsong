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
    along with SwannSong Adventure. If not, see
    <https://www.gnu.org/licenses/>.
*/

#ifndef PSTRINGS_HPP
#define PSTRINGS_HPP

#include <string>
#include <memory>

// Stores program strings
namespace pstrings {
    struct ps_data;
    typedef std::shared_ptr<ps_data> ps_data_ptr;

    ps_data_ptr init_data(std::string const& data_path,
            std::string const& lang_code);
    bool check_if_exists(pstrings::ps_data_ptr const& data,
            std::string const& id);
    std::string const& fetch_string(pstrings::ps_data_ptr const& data,
            std::string const& id);
}
#endif
