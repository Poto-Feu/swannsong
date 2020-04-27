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


#ifndef PARSER_H
#define PARSER_H

#ifdef __cplusplus

#include "room/room.h"

int parser_exec_until_end(int blockln, Room& currentRoom);

#endif

#ifdef __cplusplus
extern "C" {
#else

#include <stdbool.h>

#endif

int parser_skip_until_end(int blockln);
void parser_splitline(char* type, char* arg, char* ins);

#ifdef __cplusplus
}
#endif

#endif
