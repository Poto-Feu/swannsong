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

#ifndef PVARS_H
#define PVARS_H

extern char pvars_userlang[3];
void pvars_setstdvars(char* name, char* value);
void pvars_getstdvars(char* name, char* value);
void pvars_setgcvars(char* name, char* value);
void pvars_getgcvars(char* name, char* value);

#endif
