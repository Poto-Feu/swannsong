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
#include "../vars/pconst.h"
#include "../vars/pvars.h"
#include "../perror.h"
#include "../pstrings.h"
#include "../stringsm.h"
#include "../room/room.h"
#include "../room/find.h"

/*Execute the appropriate action according to the type and the arg received*/
void parser_execins(char* type, char* arg, bool* inblock)
{
    if(!strcmp(type, "PRINT"))
    {
        printf("\n");
        pstrings_display(arg);
        printf("\n");
    } else if (strcmp(type, "IF") == 0)
    {
        *inblock = 1;
    } else if (*inblock == 0 && !strcmp(type, "DISPLAY"))
    {    
        if(!strcmp(arg, "CHOICES"))
        {
            //To do
            int* roomln = calloc(1, sizeof(int));
            char* croomid = calloc((P_MAX_BUF_SIZE - 1), sizeof(char));
            pvars_getstdvars("currentroom", &croomid);
            find_roomline(croomid, roomln);
            free(croomid);
            free(roomln);
        }
    } else if (*inblock == 0 && strcmp(type, "END") == 0)
    {
    } else
    {
        printf("arg: %s\n", type);
        perror_disp("UNK_INS_TYPE", 0);
    }
    
}

/*Extract the type and the argument from a string*/
void parser_splitline(char** type, char** arg, char* ins)
{
    int i = 0;
    int len = 0;
    char* argtocpy = calloc(P_MAX_BUF_SIZE, sizeof(char));
    
    stringsm_chomp(ins);
    len = strlen(ins);
    ins[len] = '\0';
    stringsm_getfw(type, ins, &i);
    if(len != (int)strlen(*type))
    {
        int findex = 0;

        for(int index = i; index < len; index++)
        {
            argtocpy[findex] = ins[index];
            findex++;
        }
        strcpy(*arg, argtocpy);
    } else
    {
        strcpy(*arg, "ARGNULL");
    }

    free(argtocpy);
}
