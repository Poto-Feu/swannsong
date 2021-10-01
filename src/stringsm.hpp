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

#ifndef STRINGSM_HPP
#define STRINGSM_HPP

#include <string>

namespace stringsm
{
    const std::string empty_string = "";

    //Replace the newline character by a null terminator
    void chomp(char* str);
    //Check if the content of a string is an unsigned number
    bool is_number(std::string const& p_str);
    //Remove space and tab characters at the beginning of a string
    void rtab(std::string& p_buf);
    //Get the first word of a string
    std::string getfw(std::string const& p_str);
    //Check if the passed char array is a string enclosed with quotes
    bool is_str(std::string const& p_str);
    //Extract string from quotations marks
    std::string ext_str_quotes(std::string const& p_str);
    bool is_single_word(std::string const& p_str);
    bool has_quotes(std::string const& p_str);
}
#endif
