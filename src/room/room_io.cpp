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

extern "C"
{
#include <stdlib.h>
#include <string.h>
#include "fileio/fileio.h"
#include "vars/pconst.h"
#include "vars/pvars.h"
#include "stringsm.h"
#include "perror.h"
}

#include <iostream>
#include <string>
#include <vector>
#include "room/room_io.h"

using std::string;
using std::cout;
using std::endl;
using std::vector;

static vector<string> roomfile_arr {};

static void open_strfile(FILE** fp);
static void add_ln_to_vec(char* p_ln);

void roomio_copy_file_to_vec()
{
    char buf[P_MAX_BUF_SIZE]{0};
    FILE* fp = NULL;
    char* roomfile = NULL;

    open_strfile(&fp);
    pvars_getstdvars("roomfile", &roomfile);
    
    while(fgets(buf, P_MAX_BUF_SIZE - 1, fp) != NULL)
    {
        stringsm_chomp(buf);
        stringsm_rtab(buf);

        if(*buf != '\0') add_ln_to_vec(buf);
        else continue;
    }
    free(roomfile);
}

void roomio_fetch_ln(char** p_ln, int ind)
{
    if(*p_ln != NULL) free(*p_ln);

    if(ind > static_cast<int>(roomfile_arr.size()))
    {
        perror_disp("line index does not exist", 1);
    } else
    {
        int str_len = P_MAX_BUF_SIZE;
        string ind_ln(roomfile_arr[ind-1]);

        str_len = ind_ln.length();
        *p_ln = (char*)malloc((str_len+1) * sizeof(char));
        (*p_ln)[str_len] = '\0';
        strcpy(*p_ln, ind_ln.c_str());
    }
}

static void add_ln_to_vec(char* p_ln)
{
    string str_ln(p_ln);
    roomfile_arr.push_back(str_ln);
}

static void open_strfile(FILE** fp)
{
    char* roomfile = NULL;

    pvars_getstdvars("roomfile", &roomfile);
    fileio_setfileptr(fp, roomfile);

    free(roomfile);
}

