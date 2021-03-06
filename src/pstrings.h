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

#ifndef PSTRINGS_H
#define PSTRINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

bool pstrings_check_exist(const char* id);
void pstrings_display(const char* id);

#ifdef __cplusplus
}

#include <filesystem>
#include <string>

namespace pstrings
{
    void copy_file_to_vec(std::string const& p_langdir, std::filesystem::path const& data_path);
    std::string fetch(std::string const& id);
    void display(std::string const& id);
    bool check_exist(std::string const& id);
}

#endif

#endif
