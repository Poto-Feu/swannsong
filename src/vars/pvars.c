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


bool fetch_pvarsid(char* name, int* id);

void pvars_setgcvars(char* name, char* value)
{
    int* varfndid = malloc(sizeof(int));
    bool isvarfnd = fetch_pvarsid(name, varfndid);

    if(isvarfnd == 1 && *varfndid != -1)
    {
        pvars_gameconf[*varfndid].value = malloc((strlen(value) + 1) * sizeof (char));
        strcpy(pvars_gameconf[*varfndid].value, value);
    } else
    {
        perror_disp("UNK_GAMECONF_VAR", 0);
    }
}

void pvars_getgcvars(char* name, char* value)
{
    int *id = malloc(sizeof(int));
    bool isvarfnd = fetch_pvarsid(name, id);

    if(isvarfnd)
    {
        int vlen = strlen(pvars_gameconf[*id].value);
        char* check = realloc(value, (vlen + 1) * sizeof(char));
        if(!check)
        {
            perror_disp("REALLOC_FAIL", 1);
        }
        free(check);
        strcpy(value, pvars_gameconf[*id].value);
        free(id);
    }
    else
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

bool fetch_pvarsid(char* name, int* id)
{
    bool isvarfnd = false;
    int varfndid = -1;
    for(int i = 0; i < GCVARS_LN ; i++)
    {
        if(!strcmp(name, pvars_gameconf[i].name))
        {
            isvarfnd = true;
            varfndid = i;
            break;
        }
    }
    *id = varfndid;

    return isvarfnd;
}
