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

#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <stdbool.h>
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
void stringsm_getfw(char* fw, char* str, int* index)
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
        return;
    }
}

{



/*Extract string from quotations marks*/
void stringsm_ext_str_quotes(char* r_ext, const char* p_str)
{
    bool str_end = false;
    char quote_ch = p_str[0];

    for(int i = 1; !str_end && p_str[1] != quote_ch; i++)
    {
        r_ext[i-1] = p_str[i];

        if(p_str[i+1] == quote_ch) str_end = true;
    }
}
