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

#define PVARS_LN 1
#define GCVARS_LN 4

struct pvar
{
    char* name;
    char* value;
};

char pvars_userlang[3] = "en";
static struct pvar pvars[PVARS_LN];
static struct pvar pvars_gameconf[GCVARS_LN];


static bool fetch_pvarsid(char* name, int* id, bool isgcvar);
static bool fetch_gcvarsid(char* name, int *id);

/*Set the value of a gameconf-defined variable*/
void pvars_setgcvars(char* name, char* value)
{
    int* varfndid = malloc(sizeof(int));
    bool isvarfnd = fetch_gcvarsid(name, varfndid);

    if(isvarfnd == 1 && *varfndid != -1)
    {
        int vlen = strlen(value);
        pvars_gameconf[*varfndid].value = malloc((vlen + 1) * sizeof(char));
        strcpy(pvars_gameconf[*varfndid].value, value);
    } else
    {
        perror_disp("UNK_GAMECONF_VAR", 0);
    }
}

/*Copy the value of a gameconf-defined variable*/
void pvars_getgcvars(char* name, char* value)
{
    int *id = malloc(sizeof(int));
    bool isvarfnd = fetch_gcvarsid(name, id);

    if(isvarfnd)
    {
        int vlen = strlen(pvars_gameconf[*id].value);
        char* check = realloc(value, (vlen + 1) * sizeof(char));
        if(!check)
        {
            perror_disp("REALLOC_FAIL", 1);
        }
        strcpy(value, pvars_gameconf[*id].value);
    }
    else
    {
        perror_disp("GAMECONF_VAR_NF", 1);
    }
    free(id);
}

void init_gcvars()
{
    pvars_gameconf[0].name = "langdir";
    pvars_gameconf[1].name = "roomfile";
    pvars_gameconf[2].name = "defaultlang";
    pvars_gameconf[GCVARS_LN - 1].name = "firstroom";
}

/*Fetch regular pvars id*/
static bool fetch_stdpvarsid(char* name, int *id)
{
    return fetch_pvarsid(name, id, true);
}

/*Fetch gameconf-defined pvars id*/
static bool fetch_gcvarsid(char* name, int *id)
{
    return fetch_pvarsid(name, id, true);
}


static bool fetch_pvarsid(char* name, int* id, bool isgcvar)
{
    bool isvarfnd = false;
    int varfndid = -1;
    int arrlen = 0;

    if(isgcvar == true)
    {
        arrlen = GCVARS_LN;
    } else
    {
        arrlen = PVARS_LN;
    }

    for(int i = 0; i < arrlen; i++)
    {
        char* iname = NULL;
        if(isgcvar == true)
        {
            iname = malloc(strlen(pvars_gameconf[i].name) * sizeof(char));
        }
        else
        {
            iname = malloc(strlen(pvars_gameconf[i].name) * sizeof(char));
        }
        if(!strcmp(name, pvars_gameconf[i].name))
        {
            isvarfnd = true;
            varfndid = i;
            break;
        }
        free(iname);
    }
    *id = varfndid;

    return isvarfnd;
}
