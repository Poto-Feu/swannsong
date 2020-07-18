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

extern "C" {
#include <curses.h>
}

#include <string>
#include "stringsm.h"

namespace stringsm
{
    //Return an uppercased string
    std::string to_upper(std::string p_str)
    {
        for(auto& it : p_str) it = toupper(it);
        return p_str;
    }

    //Return a lowercased string
    std::string to_lower(std::string p_str)
    {
        for(auto& it : p_str) it = tolower(it);
        return p_str;
    }

    //Check if the content of a string is an unsigned number
    bool is_number(std::string const& p_str)
    {
        if(p_str.size() == 0) return false;

        for(auto const& it : p_str) {
            if(!isdigit(it)) return false;
        }

        return true;
    }

    //Remove space and tab characters at the beginning of a string
    void rtab(std::string& p_buf)
    {
        while(p_buf[0] == '\t' || p_buf[0] == ' ') p_buf.erase(0, 1);
    }

    //Get the first word of a string
    std::string getfw(std::string const& p_str)
    {
        unsigned int i = 0;
        unsigned int str_size = p_str.size();
        std::string fw;

        for(auto& it : p_str) {
            if(it == ' ' || i + 1 == str_size) {
                if(i + 1 == str_size) ++i;
                fw = p_str.substr(0, i);
                break;
            }
            ++i;
        }
        return fw;
    }

    //Check if the passed char array is a string enclosed with quotes
    bool is_str(std::string const& p_str)
    {
        int str_size = p_str.size();
        char quote_ch = '\0';

        if(p_str.at(0) != '"' && p_str.at(0) != '\'') return false;
        else quote_ch = p_str.at(0);

        for(int i = 1; i < str_size; ++i) {
            if(p_str[i] == '\\') {
                if(p_str[i+1] == quote_ch) ++i;
            } else if(p_str[i] == quote_ch) {
                if(i + 1 < str_size) {
                    for(int j = i + 1; j < str_size; ++j) {
                        if(p_str.at(j) != ' ' && p_str.at(j) != '\t') return false;
                    }
                    return true;
                } else return true;
            }
        }
        return false;
    }

    //Extract string from quotations marks
    std::string ext_str_quotes(std::string const& p_str)
    {
        bool str_end = false;
        char quote_ch = p_str[0];
        std::string rtr_val;

        for(int i = 1; !str_end && p_str[1] != quote_ch; ++i) {
            rtr_val += p_str[i];

            if(p_str[i] == '\\' && p_str[i+1] == quote_ch) {
                rtr_val += quote_ch;
                ++i;
            } else if(p_str[i+1] == quote_ch) str_end = true;
        }
        return rtr_val;
    }
}

//Replace the newline character by a null terminator (deprecated, use std::string instead)
void stringsm_chomp(char* str)
{
    while (*str != '\n' && *str != '\0') str++;
    *str = '\0';
}
