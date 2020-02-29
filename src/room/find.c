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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../vars/pconst.h"
#include "../perror.h"
#include "../pstrings.h"
#include "../stringsm.h"
#include "../fileio/fileio.h"
#include "../fileio/parser.h"

void find_roomline(char* id, int* ln)
{
    char* roomline = malloc(P_MAX_BUF_SIZE * sizeof(char));
    int idlen = strlen(id);

    *roomline = '\0';
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


_Bool find_insline(int* foundln, int ln, char* ins)
{
    _Bool inchoices = 0;
    _Bool inonechoice = 0;
    char* buf = malloc(P_MAX_BUF_SIZE * sizeof(char));
    char* type = malloc(P_MAX_BUF_SIZE - 2 * sizeof(char));
    char* arg = malloc(P_MAX_BUF_SIZE - 2 * sizeof(char));
    FILE* fp = fopen("txt/rooms.txt", "r");
    
    *buf = '\0';
    *arg = '\0';
    *type = '\0';
    fileio_gotoline(&fp, ln);
    for(int i = 0; fgets(buf, P_MAX_BUF_SIZE - 1, fp) != NULL; i++)
    {
        stringsm_chomp(buf);
        stringsm_rtab(buf);
        
        parser_splitline(type, arg, buf);
        if (strcmp(type, ins) == 0)
        {
            *foundln = i + ln;
            return 1;
        } else if(strcmp(type, "CHOICES") == 0)
        {
            if(strcmp(ins, "CHOICES") != 0)
            {
                inchoices = 1;
            }
            
        }
        if(strcmp(type, "END") == 0)
        {
            if(inchoices == 0)
            {
                if(strcmp(type, "ATLAUNCH") == 0)
                {
                    perror_disp("NO_ATLAUNCH_INS", 0);
                    break;
                } else if(strcmp(type, "CHOICES") == 0)
                {
                    perror_disp("NO_CHOICES_INS", 0);
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
    }
    fclose(fp);
    free(type);
    free(arg);
    return 0;
}

void find_atlaunchline(int* foundln, int ln, _Bool* atlfound)
{
    char* ins = "ATLAUNCH";
    *atlfound = find_insline(foundln, ln, ins);
}

void find_choicesline(int* foundln, int ln, _Bool* choiceslfound)
{
    char* ins = "CHOICES";
    *choiceslfound = find_insline(foundln, ln, ins);
}
