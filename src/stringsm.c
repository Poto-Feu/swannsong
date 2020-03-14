/*
    Copyright (C) 2020 Adrien Saad

    This file is part of SwannSong.

    SwannSong is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    SwannSong is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
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
void stringsm_getfw(char** fw, char* str, int* index)
{
    bool space = false;
    int len = strlen(str);

    for(int i = 0; i < len; i++)
    {
        if(str[i] == ' ')
        {
            strncpy(*fw, str, i);
            *index = i+1;
            space = true;
            break;
        }
    }

    if(!space)
    {
        strcpy(*fw, str);
        *index = 0;
        return;
    }
}

void stringsm_getuseri(char** buf)
{
    char *c = NULL;

    fgets(*buf, (sizeof(*buf) + 1), stdin);
    if((c = strchr(*buf, '\n')))
    {
        stringsm_chomp(*buf);
    } else
    {
        scanf("%*[^\n]");
        scanf("%*c");
    }
    
}
