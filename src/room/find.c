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
#include <ctype.h>
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
    bool is_end = false;
    int currln = startln + 1;

    if(num < 0 || num > 9) perror_disp("choice number not allowed", 1);

    for(int i = currln; !choicefound && !is_end; i++)
    {
        char* buf = NULL;

        bool is_eof = !roomio_fetch_ln(&buf, i);

        if(is_eof) perror_disp("find_onechoiceline has hit EOF", 1);
        if(buf[0] == 'c' && buf[1] == num + '0' && buf[2] == '\0')
        {
            *ln = i;
            choicefound = true;
        } else if(buf[0] == 'c' && buf[1] != num + '0' && buf[2] == '\0')
        {
            i = parser_skip_until_end(i);
        } else if(!strcmp(buf, "END")) is_end = true;
        else 
        {
            free(buf);
            perror_disp("wrong input in find_onechoiceline", 1);
        }

        free(buf);
    }
    return choicefound;
}

/*Fetch the line where a specific block is present beginning from a 
specified line*/
bool find_blockline(int* foundln, int p_ln, const char* ins)
{
    bool is_fnd = false;
    bool is_end = false;
    bool in_choices = false;
    int currln = p_ln + 1;
    
    for(int i = currln; !is_fnd && !is_end; i++)
    {
        char* buf = NULL;
        bool is_eof = !roomio_fetch_ln(&buf, i);

        if(is_eof) perror_disp("find_blockline has hit EOF", true);
        if(!strcmp(ins, buf) && !in_choices)
        {
            is_fnd = true;
            *foundln = i;
        } else if(!strcmp("ATLAUNCH", buf))
        {
            if(in_choices)
            {
                free(buf);
                perror_disp("ATLAUNCH instruction inside CHOICES list", true);
            }
            else i = parser_skip_until_end(i);
        } else if(!strcmp("CHOICES", buf))
        {
            if(in_choices)
            {
                free(buf);
                perror_disp("CHOICES instruction inside CHOICES list", true);
            } else in_choices = true;
        } else if(buf[0] == 'c' && isdigit(buf[1]) && buf[2] == '\0')
        {
            if(in_choices) i = parser_skip_until_end(i);
            else
            {
                free(buf);
                perror_disp("CHOICE block outside CHOICES list", true);
            }
        } else if(!strcmp("END", buf))
        {
            if(in_choices) in_choices = false;
            else is_end = true;
        }

        free(buf);
    }

    return is_fnd;
}
