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
#include "init.h"
#include "vars/pconst.h"
#include "vars/pvars.h"
#include "perror.h"
#include "pstrings.h"
#include "exitgame.h"
#include "room/room.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main (void)
{
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif

    char* room_name = calloc(P_MAX_BUF_SIZE, sizeof(char));

    init_game();
    pvars_getgcvars("firstroom", room_name);
    room_load(room_name);
    free(room_name);
    exitgame(0);
}
