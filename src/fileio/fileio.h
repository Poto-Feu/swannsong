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

#ifndef FILEIO_H
#define FILEIO_H

#ifdef __cplusplus
extern "C" {
#include <cstdio>
#else
#include <stdio.h>
#endif

void fileio_setfileptr(FILE** fp, const char* path);
char* fileio_getfileln(char* buf, int size, FILE** ptr);

#ifdef __cplusplus
}

#include <string>
#include <fstream>

namespace fileio
{
    bool getfileln(std::string& r_str, std::ifstream& p_stream);
}
#endif

#endif
