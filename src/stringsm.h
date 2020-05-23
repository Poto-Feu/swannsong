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

#ifndef STRINGSM_H
#define STRINGSM_H

#ifdef __cplusplus
extern "C" {
#endif

void stringsm_chomp(char* str);

#ifdef __cplusplus
}

#include <string>

void stringsm_ext_str_quotes(std::string& r_ext, const char* p_str);

namespace stringsm
{
    std::string to_upper(std::string p_str);
    bool is_number(std::string const p_str);
    void rtab(std::string& p_buf);
    std::string getfw(std::string p_str);
    bool is_str(std::string const p_str);
    std::string ext_str_quotes(std::string const p_str);
}
#endif

#endif
