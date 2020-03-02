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

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "pvars.h"
#include "../perror.h"

#define GCVARS_LN 3

struct gcvar 
{
    char* name;
    char* value;
};

char pvars_userlang[3] = "en";
struct gcvar pvars_gameconf[3];


void pvars_setgcvars(char* name, char* value)
{
    bool isvarfnd = 0;
    int varfndid = -1;
    for(int i = 0; i < GCVARS_LN ; i++)
    {
        if(!strcmp(name, pvars_gameconf[i].name))
        {
            isvarfnd = 1;
            varfndid = i;
            break;
        }
    }

    if(isvarfnd == 1 && varfndid != -1)
    {
        pvars_gameconf[varfndid].value = malloc((strlen(value) + 1) * sizeof (char));
        strcpy(pvars_gameconf[varfndid].value, value);
    } else
    {
        perror_disp("UNK_GAMECONF_VAR", 0);
    }
}

void init_gcvars()
{
    pvars_gameconf[0].name = "langdir";
    pvars_gameconf[1].name = "roomfile";
    pvars_gameconf[2].name = "firstroom";
}
