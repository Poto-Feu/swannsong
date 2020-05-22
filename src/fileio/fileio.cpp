/*
    Copyright (C) 2020 Adrien Saad

    This file is part of SwannSong Adventure.

    SwannSong Adventure is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SwannSong Adventure is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong Adventure.  If not, see <https://www.gnu.org/licenses/>.
*/

extern "C" {
#include "perror.h"
}

#include <string>
#include <fstream>
#include "fileio.h"

namespace fileio
{
    void check_ln_length(char* buf, int size)
    {
        int newline_fnd = false;

        for(int i = 0; i < size; i++)
        {
            if(buf[i] == '\n') newline_fnd = true;
        }

        if(!newline_fnd) perror_disp("file string is too long", true);
    }

    bool getfileln(std::string& r_str, std::ifstream& p_stream)
    {
        if(std::getline(p_stream, r_str)) return true;
        else return false;
    }
}

void fileio_setfileptr(FILE** fp, const char* path)
{
    *fp = fopen(path, "r");
    if(*fp == NULL) perror_disp("file cannot be open", 1);
}

char* fileio_getfileln(char* buf, int size, FILE** ptr)
{
    char* rtrn_val = fgets(buf, size, *ptr);

    if(rtrn_val != NULL) fileio::check_ln_length(buf, size);

    return rtrn_val;
}
