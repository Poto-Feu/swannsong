/*
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
    _Bool isvarfnd = 0;
    int varfndid = -1;
    for(int i = 0; i < GCVARS_LN ; i++)
    {
        if(strcmp(name, pvars_gameconf[i].name) == 0)
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

void pvars_getgcvars(char* name, char* value)
{
    _Bool isvarfnd = 0;
    for(int i = 0; i < GCVARS_LN; i++)
    {
        if(!strcmp(name, pvars_gameconf[i].name))
        {
            strcpy(value, pvars_gameconf[i].value);
            isvarfnd = 1;
            break;
        }
    }
    if(!isvarfnd)
    {
        perror_disp("GAMECONF_VAR_NF", 1);
    }
}

void init_gcvars()
{
    pvars_gameconf[0].name = "langdir";
    pvars_gameconf[1].name = "roomfile";
    pvars_gameconf[2].name = "firstroom";
}
