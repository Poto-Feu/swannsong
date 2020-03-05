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
#include "../perror.h"
#include "../pstrings.h"
#include "../stringsm.h"
#include "../fileio/fileio.h"
#include "../fileio/parser.h"

void find_roomline(char* id, int* ln)
{
    char* roomline = calloc(P_MAX_BUF_SIZE, sizeof(char));
    int idlen = strlen(id);

    strcpy(roomline, "ROOM");
    roomline[strlen(roomline)] = ' ';
    for(int i = 0; i < idlen; i++)
    {
        roomline[strlen(roomline)] = id[i];
    }
    fileio_getln(ln, roomline);
    free(roomline);
    roomline = NULL;
}


bool find_insline(int* foundln, int ln, char* ins)
{
    bool inchoices = false;
    bool inonechoice = false;
    char* buf = calloc(P_MAX_BUF_SIZE, sizeof(char));
    FILE* fp = fopen("txt/rooms.txt", "r");
    
    fileio_gotoline(&fp, ln);
    for(int i = 0; fgets(buf, P_MAX_BUF_SIZE - 1, fp) != NULL; i++)
    {
        char* type = calloc((P_MAX_BUF_SIZE - 2), sizeof(char));
        char* arg = calloc((P_MAX_BUF_SIZE - 2), sizeof(char));
        stringsm_chomp(buf);
        stringsm_rtab(buf);
        
        parser_splitline(&type, &arg, buf);
        if (!strcmp(type, ins))
        {
            *foundln = i + ln;
            free(type);
            free(arg);
            fclose(fp);
            free(buf);
            return 1;
        } else if(!strcmp(type, "CHOICES"))
        {
            if(strcmp(ins, "CHOICES"))
            {
                inchoices = 1;
            }
            
        }
        free(arg);
        if(!strcmp(type, "END"))
        {
            if(inchoices == 0)
            {
                if(!strcmp(type, "ATLAUNCH"))
                {
                    perror_disp("NO_ATLAUNCH_INS", 0);
                    free(type);
                    break;
                } else if(!strcmp(type, "CHOICES"))
                {
                    perror_disp("NO_CHOICES_INS", 0);
                    free(type);
                    break;
                }
            } else if(inonechoice == 1)
            {
                inonechoice = 0;
            } else if(inonechoice == 0 && inchoices == 1)
            {
                inchoices = 0;
            }
        }
        free(type);
    }
    fclose(fp);
    free(buf);
    return 0;
}

void find_atlaunchline(int* foundln, int ln, bool* atlfound)
{
    char* ins = "ATLAUNCH";
    *atlfound = find_insline(foundln, ln, ins);
}

void find_choicesline(int* foundln, int ln, bool* choiceslfound)
{
    char* ins = "CHOICES";
    *choiceslfound = find_insline(foundln, ln, ins);
}
