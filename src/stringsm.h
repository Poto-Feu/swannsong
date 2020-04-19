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

#ifndef STRINGSM_H
#define STRINGSM_H

void stringsm_chomp(char* str);
void stringsm_rtab(char* str);
void stringsm_getuseri(char** buf);
void stringsm_getfw(char** fw, char* s, int *index);
void stringsm_ext_str_quotes(char** r_ext, char* p_str);

#endif
