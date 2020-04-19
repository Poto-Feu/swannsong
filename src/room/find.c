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
#include "fileio/fileio.h"
#include "interpreter/parser.h"
#include "room/room_io.h"
#include "perror.h"
#include "pstrings.h"
#include "stringsm.h"

bool find_blockline(int* foundln, int ln, const char* ins);

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
    if(!roomio_find_ind(ln, roomline))
    {
        perror_disp("room not found in file", true);
    }
    free(roomline);
}

/*Fetch the line where a Choice declaration start ; the startln must point to
the beginning of the Choices decleration set*/
bool find_onechoiceline(int num, int startln, int* ln)
{
    bool choicefound = false;
    bool inonechoice = false;
    bool inif = false;
    int currln = startln + 1;

    if(num < 0 || num > 9) perror_disp("WRG_CHOI_NUM", 1);

    for(int i = currln; !choicefound; i++)
    {
        char* buf = NULL;

        roomio_fetch_ln(&buf, i);

        if(buf[0] == 'c' && buf[1] == num + '0')
        {
            if(inif == false && inonechoice == false)
            {
                *ln = i;
                choicefound = true;
            } else
            {
                free(buf);
                perror_disp("WRG_CHOICE_PLACE", 1);
            }
        } else if(buf[0] == 'c' && buf[1] != num + '0') inonechoice = true;
        else if(buf[0] == 'I' && buf[1] == 'F') inif = true;
        else if(buf[0] == 'E' && buf[1] == 'N' && buf[2] == 'D')
        {
            if(inif) inif = false;
            else if (inonechoice) inonechoice = false;
            else
            {
                free(buf);
                break;
            }
        }

    free(buf);
    }
    return choicefound;
}

/*Fetch the first line ecountered where a specific instruction is present
beginning from a specified line*/
bool find_blockline(int* foundln, int p_ln, const char* ins)
{
    bool is_fnd = false;
    int currln = p_ln + 1;
    
    for(int i = currln; !is_fnd; i++)
    {
        char* buf = NULL;
        bool is_eof = !roomio_fetch_ln(&buf, i);

        if(is_eof) break;
        if(!strcmp(ins, buf))
        {
            is_fnd = true;
            *foundln = i;
        }

        free(buf);
    }

    return is_fnd;
}
