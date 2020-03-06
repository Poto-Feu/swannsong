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

#ifndef FILEIO_H
#define FILEIO_H

void fileio_setfileptr(FILE** fp, char* path);
void fileio_gotoline(FILE** fp, int ln);
void fileio_getln(int* ln, char* s);
void fileio_execuntilend(int startln);

#endif
