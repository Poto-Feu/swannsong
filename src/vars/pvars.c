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
#include <stdbool.h>
#include "pvars.h"
#include "pconst.h"
#include "perror.h"

#define STDVARS_LN 3
#define GCVARS_LN 4

/*pvar is short for program var - not named var to prevent naming conflict*/
typedef struct 
{
    char* name;
    char* value;
} pvar;

static pvar stdvars[STDVARS_LN] =
{
    {"lang", NULL},
    {"nextroom", NULL},
    {"roomfile", NULL}
};
static pvar gcvars[GCVARS_LN] =
{
    {"langdir", NULL},
    {"roomfile", NULL},
    {"defaultlang", NULL},
    {"firstroom", NULL}
};


static bool fetch_pvarsid(const char* name, int* id, bool isgcvar);
static void pvars_setpvars(const char* name, char* value, bool isgcvar);
static void pvars_getpvars(const char* name, char** value, bool isgcvar);


/*Set the value of a standard program variable*/
void pvars_setstdvars(const char* name, char* value)
{
    pvars_setpvars(name, value, false);
}

/*Set the value of a gameconf-defined variable*/
void pvars_setgcvars(const char* name, char* value)
{
    pvars_setpvars(name, value, true);
}

static void pvars_setpvars(const char* name, char* value, bool isgcvar)
{
    int* varfndid = calloc(1, sizeof(int));
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
void pvars_getstdvars(const char* name, char** value)
{
    pvars_getpvars(name, value, false);
}

/*Copy the value of a gameconf-defined variable*/
void pvars_getgcvars(const char* name, char** value)
{
    pvars_getpvars(name, value, true);
}

static void pvars_getpvars(const char* name, char** value, bool isgcvar)
{
    int id = -1;
    bool isvarfnd = fetch_pvarsid(name, &id, isgcvar);

    if(isvarfnd)
    {
        int vlen = 0;
        char* valuetocpy = calloc(P_MAX_BUF_SIZE, sizeof(char));

        if(isgcvar) strcpy(valuetocpy, gcvars[id].value);
        else strcpy(valuetocpy, stdvars[id].value);

        vlen = strlen(valuetocpy);

        if(*value != NULL) free(*value);

        *value = calloc((vlen+1), sizeof(char));
        strcpy(*value, valuetocpy);

        free(valuetocpy);
    }
    else
    {
        if(isgcvar) perror_disp("gameconf var does not exist", 1);
        else perror_disp("std var does not exist", 1);
    }
}

static bool fetch_pvarsid(const char* name, int* id, bool isgcvar)
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

void pvars_freegcvar(const char* name)
{
    int id = 0;
    bool pvarsid_check = fetch_pvarsid(name, &id, true);

    if(pvarsid_check)
    {
        if(gcvars[id].value)
        {
            free(gcvars[id].value);
            gcvars[id].value = NULL;
        } else
        {
            perror_disp("GAMECONF_VAR_NOTALLOC", 0);
        }
    } else
    {
        perror_disp("UNK_GAMECONF_VAR (in pvars_freegcvar)", 0);
    }
}
