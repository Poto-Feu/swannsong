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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "gameconf.h"
#include "../vars/pconst.h"
#include "../vars/pvars.h"
#include "../stringsm.h"

void gameconf_splitins(char* var, char* value, char* ins);

void gameconf_readfile()
{
    FILE* fp = fopen("gameconf.txt", "r");
    char* buf = malloc(P_MAX_BUF_SIZE * sizeof(char));
    
    *buf = '\0';

    while (fgets(buf, (P_MAX_BUF_SIZE - 1), fp) != NULL)
    {
        char* var = malloc((P_MAX_BUF_SIZE - 1) * sizeof(char));
        char* value = malloc((P_MAX_BUF_SIZE - 1) * sizeof(char));

        stringsm_chomp(buf);
        stringsm_rtab(buf);
        if(strcmp(buf, "") && buf[0] != '*')
        {
            gameconf_splitins(var, value, buf);
            pvars_setgcvars(var, value);
        } 

        free(var);
        free(value);
    }

    fclose(fp);
    free(buf);
}

void gameconf_splitins(char* var, char* value, char* ins)
{
    int index = 0;
    int vindex = 0;
    bool quoteinc = 0;

    *var = '\0';
    *value = '\0';

    for(int i = 0; ins[i] != '='; i++)
    {
        var[i] = ins[i];
        index = i+1;
    }
    for(int i = index; ins[i] != '\0'; i++)
    {
        if(quoteinc == 1)
        {
            if(ins[i] == '"')
            {
                break;
            } else
            {
                value[vindex] = ins[i];
                vindex++;
            }
        }
        else 
        {
            if(ins[i] == '"')
            {
                quoteinc = 1;
            }
        }
    }
}
