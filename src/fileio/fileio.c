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
#include "fileio.h"
#include "vars/pconst.h"
#include "vars/pvars.h"
#include "interpreter/parser.h"
#include "perror.h"
#include "stringsm.h"

void fileio_setfileptr(FILE** fp, char* path)
{
    *fp = fopen(path, "r");
    if(*fp == NULL)
    {
        perror_disp("FOPEN_NULL", 1);
    }
}

void fileio_gotoline(FILE** fp, int ln)
{
    char* buf = calloc(P_MAX_BUF_SIZE, sizeof(char));

    for(int i = 0; i < ln; i++)
    {
        if(fgets(buf, P_MAX_BUF_SIZE - 1, *fp) == NULL)
        {
            perror_disp("FILE_LINE_NF", 1);
        } else
        {
            stringsm_chomp(buf);
            stringsm_rtab(buf);
        }
    }
    free(buf);
}

void fileio_getln(int* ln, char* s)
{
    char* roomfile = calloc((P_MAX_BUF_SIZE-1), sizeof(char));
    FILE* fp = NULL;
    char* buf = calloc(P_MAX_BUF_SIZE, sizeof(char));
    int i = 0;

    pvars_getgcvars("roomfile", &roomfile);
    fp = fopen(roomfile, "r");
    free(roomfile);
    *ln = 0;
    while(fgets(buf, P_MAX_BUF_SIZE - 1, fp) != NULL)
    {
        stringsm_chomp(buf);
        stringsm_rtab(buf);
        if(!strcmp(s, buf))
        {
            *ln = i + 1;
            break;
        }
        i++;
    }
    fclose(fp);
    free(buf);
}
