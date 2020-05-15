/*
    Copyright (C) 2020 Adrien Saad

    This file is part of SwannSong.

    SwannSong is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SwannSong is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong.  If not, see <https://www.gnu.org/licenses/>.
*/

extern "C" {
#include <curses.h>
}

#include <cstdlib>
#include <cstring>
#include <string>
#include "stringsm.h"

void stringsm_chomp(char* str)
{
    while (*str != '\n' && *str != '\0')
        str++;

    *str = '\0';
}

void stringsm_rtab(char* str)
{
    while(str[0] == '\t' || str[0] == ' ')
    {
        for(int i = 0; i < (int)strlen(str); i++)
        {
            str[i] = str[i+1];
        }
    }
}

/*Get the first word of a string*/
void stringsm_getfw(char* fw, const char* str, int* index)
{
    bool space = false;
    int len = strlen(str);

    for(int i = 0; i < len; i++)
    {
        if(str[i] == ' ')
        {
            strncpy(fw, str, i);
            *index = i+1;
            space = true;
            break;
        }
    }

    if(!space)
    {
        strcpy(fw, str);
        *index = 0;
    }
}

/*Check if the passed char array is a string enclosed with quotes*/
bool stringsm_is_str(const char* p_str)
{
    bool is_str = false;
    bool nulterm = false;
    char quote_ch = '\0';

    if(p_str[0] != '"' && p_str[0] != '\'') return false;
    else quote_ch = p_str[0];

    for(int i = 1; !nulterm && !is_str ; ++i)
    {
        if(p_str[i] == '\0') nulterm = true;
        else if(p_str[i] == '\\')
        {
            if(p_str[i+1] == quote_ch) ++i;
        } else if(p_str[i] == quote_ch)
        {
            bool space_end = true;

            if(p_str[i+1] != '\0')
            {
                for(int j = i + 1; space_end && !nulterm; j++)
                {
                    if(p_str[j] != '\0') nulterm = true;
                    else if(p_str[j] != ' ' || p_str[j] != '\t') 
                    {
                        space_end = false;
                    }
                }
            }
            is_str = space_end;
        }
    }

    return is_str;
}

/*Extract string from quotations marks*/
void stringsm_ext_str_quotes(std::string& r_ext, const char* p_str)
{
    auto str_end = false;
    auto quote_ch = p_str[0];

    for(int i = 1; !str_end && p_str[1] != quote_ch; i++)
    {
        r_ext += p_str[i];

        if(p_str[i] == '\\' && p_str[i+1] == quote_ch)
        {
            r_ext += quote_ch;
            ++i;
        } else if(p_str[i+1] == quote_ch) str_end = true;
    }
}
