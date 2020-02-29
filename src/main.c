/*
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

    char* next_room = malloc(100*sizeof(char));
    char* room_name = malloc(200*sizeof(char));

    init_game();
    strcpy(room_name, "menu");
    room_load(room_name);
    free(next_room);
    free(room_name);
    exitgame(0);
}
