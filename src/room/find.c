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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vars/pconst.h"
#include "vars/pvars.h"
#include "perror.h"
#include "pstrings.h"
#include "stringsm.h"
#include "fileio/fileio.h"
#include "interpreter/parser.h"

bool find_blockline(int* foundln, int ln, char* ins);

/*Return the line where the ATLAUNCH block start*/
bool find_atlaunchline(int* foundln, int ln)
{
    char* ins = "ATLAUNCH";
    return find_blockline(foundln, ln, ins);
}

/*Return the line where the CHOICES block start*/
bool find_choicesline(int* foundln, int ln)
{
    char* ins = "CHOICES";
    return find_blockline(foundln, ln, ins);
}

/*Fetch the beginning line of the room definition in file*/
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
}

/*Fetch the line where a Choice declaration start ; the startln must point to
the beginning of the Choices decleration set*/
bool find_onechoiceline(int num, int startln, int* ln)
{
    bool choicefound = false;
    bool inonechoice = false;
    bool inif = false;
    char* buf = calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* roomfile = calloc(P_MAX_BUF_SIZE, sizeof(char));
    FILE* fp = NULL;

    if(num < 0 || num > 9)
    {
        free(buf);
        free(roomfile);
        perror_disp("WRG_CHOI_NUM", 1);
    }
    pvars_getgcvars("roomfile", &roomfile);
    fileio_setfileptr(&fp, roomfile);
    fileio_gotoline(&fp, startln);

    free(roomfile);

    for(int i = 0; !choicefound; i++)
    {
        fgets(buf, (P_MAX_BUF_SIZE - 1), fp);
        stringsm_chomp(buf);
        stringsm_rtab(buf);

        if(buf[0] == 'c' && buf[1] == num + '0')
        {
            if(inif == false && inonechoice == false)
            {
                *ln = i + startln;
                choicefound = true;
            } else
            {
                free(buf);
                perror_disp("WRG_CHOICE_PLACE", 1);
            }
        } else if(buf[0] == 'c' && buf[1] != num + '0')
        {
            inonechoice = true;
        } else if(buf[0] == 'I' && buf[1] == 'F')
        {
            inif = true;
        } else if(buf[0] == 'E' && buf[1] == 'N' && buf[2] == 'D')
        {
            if(inif)
            {
                inif = false;
            } else if (inonechoice)
            {
                inonechoice = false;
            } else
            {
                break;
            }
        }
    }

    free(buf);
    return choicefound;
}

/*Fetch the line where a specific instruction is present beginning from a
specified line*/
bool find_blockline(int* foundln, int ln, char* ins)
{
    bool inchoices = false;
    bool inonechoice = false;
    char* roomfile = calloc((P_MAX_BUF_SIZE - 1), sizeof(char));
    char* buf = calloc(P_MAX_BUF_SIZE, sizeof(char));
    FILE* fp = NULL;

    pvars_getgcvars("roomfile", &roomfile);
    fileio_setfileptr(&fp, roomfile);
    fileio_gotoline(&fp, ln);

    free(roomfile);

    for(int i = 0; fgets(buf, P_MAX_BUF_SIZE - 1, fp) != NULL; i++)
    {
        char* block = calloc((P_MAX_BUF_SIZE - 2), sizeof(char));
        int index = -1;

        stringsm_chomp(buf);
        stringsm_rtab(buf);        
        stringsm_getfw(&block, buf, &index);
        if (!strcmp(block, ins))
        {
            *foundln = i + ln;

            free(block);
            fclose(fp);
            free(buf);

            return 1;
        } else if(!strcmp(block, "CHOICES"))
        {
            if(strcmp(ins, "CHOICES"))
            {
                inchoices = 1;
            }
            
        }

        if(!strcmp(block, "END"))
        {
            if(inchoices == 0)
            {
                if(!strcmp(block, "ATLAUNCH"))
                {
                    perror_disp("NO_ATLAUNCH_INS", 0);
                    free(block);

                    break;
                } else if(!strcmp(block, "CHOICES"))
                {
                    perror_disp("NO_CHOICES_INS", 0);
                    free(block);

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

        free(block);
    }
    fclose(fp);
    free(buf);

    return 0;
}

