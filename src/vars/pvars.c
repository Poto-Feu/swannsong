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

#define STDVARS_LN 3
#define GCVARS_LN 4

struct pvar
{
    char* name;
    char* value;
};

char pvars_userlang[3] = "en";
static struct pvar stdvars[STDVARS_LN] =
{
    {.name = "lang", .value = NULL},
    {.name = "currentroom", .value = NULL},
    {.name = "nextroom", .value = NULL}
};
static struct pvar gcvars[GCVARS_LN] =
{
    {.name = "langdir", .value = NULL},
    {.name = "roomfile", .value = NULL},
    {.name = "defaultlang", .value = NULL},
    {.name = "firstroom", .value = NULL}
};


static bool fetch_pvarsid(char* name, int* id, bool isgcvar);
static void pvars_setpvars(char* name, char* value, bool isgcvar);
static void pvars_getpvars(char* name, char* value, bool isgcvar);

/*Set the value of a standard program variable*/
void pvars_setstdvars(char* name, char* value)
{
    pvars_setpvars(name, value, false);
}

/*Set the value of a gameconf-defined variable*/
void pvars_setgcvars(char* name, char* value)
{
    pvars_setpvars(name, value, true);
}

static void pvars_setpvars(char* name, char* value, bool isgcvar)
{
    int* varfndid = malloc(sizeof(int));
    bool isvarfnd = fetch_pvarsid(name, varfndid, isgcvar);
    if(isvarfnd == 1 && *varfndid != -1)
    {
        int vlen = strlen(value) + 1;
        if(isgcvar)
        {
            if(gcvars[*varfndid].value != NULL)
            {
                free(gcvars[*varfndid].value);
            }
            gcvars[*varfndid].value = calloc(vlen, sizeof(char));
            strcpy(gcvars[*varfndid].value, value);
        } else
        {
            if(stdvars[*varfndid].value != NULL)
            {
                free(stdvars[*varfndid].value);
            }
            stdvars[*varfndid].value = calloc(vlen, sizeof(char));
            strcpy(stdvars[*varfndid].value, value);
        }
        
    } else
    {
        if(isgcvar)
        {
            perror_disp("UNK_GAMECONF_VAR", 0);
        }
        else
        {
            perror_disp("UNK_STD_VAR", 0);
        }
    }
    
    free(varfndid);
}

/*Copy the value of a standard program variable*/
void pvars_getstdvars(char* name, char* value)
{
    pvars_getpvars(name, value, false);
}

/*Copy the value of a gameconf-defined variable*/
void pvars_getgcvars(char* name, char* value)
{
    pvars_getpvars(name, value, true);
}

static void pvars_getpvars(char* name, char* value, bool isgcvar)
{
    int *id = malloc(sizeof(int));
    bool isvarfnd = fetch_pvarsid(name, id, isgcvar);
    if(isvarfnd)
    {
        int vlen = 0;
        if(isgcvar)
        {
            vlen = strlen(gcvars[*id].value);
        } else
        {
            vlen = strlen(stdvars[*id].value);
        }
        
        char* check = realloc(value, (vlen + 1) * sizeof(char));
        if(!check)
        {
            perror_disp("REALLOC_FAIL", 1);
        }
        if(isgcvar)
        {
            strcpy(value, gcvars[*id].value);
        } else
        {
            strcpy(value, stdvars[*id].value);
        }
    }
    else
    {
        if(isgcvar)
        {
            perror_disp("GAMECONF_VAR_NF", 1);
        }
        else
        {
            perror_disp("STD_VAR_NF", 1);
        }
    }

    free(id);
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
        arrlen = STDVARS_LN;
    }

    for(int i = 0; i < arrlen; i++)
    {
        char* iname = NULL;
        if(isgcvar)
        {
            iname = calloc(strlen(gcvars[i].name), sizeof(char));
            if(!strcmp(name, gcvars[i].name))
            {
                isvarfnd = true;
                varfndid = i;
                free(iname);
                break;
            }
        }
        else
        {
            iname = calloc(strlen(stdvars[i].name), sizeof(char));
            if(!strcmp(name, stdvars[i].name))
            {
                isvarfnd = true;
                varfndid = i;
                free(iname);
                break;
            }
        }
        free(iname);
    }
    *id = varfndid;

    return isvarfnd;
}
