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
#include <stdbool.h>
#include "room.h"
#include "find.h"
#include "vars/pconst.h"
#include "vars/pvars.h"
#include "perror.h"
#include "pstrings.h"
#include "stringsm.h"
#include "fileio/fileio.h"
#include "fileio/parser.h"

/*Read the first ATLAUNCH block encountered starting from specified line*/
static void room_atlaunch(int* roomln)
{
    int *foundln = calloc(1, sizeof(int));
    bool atlfound = false;

    atlfound = find_atlaunchline(foundln, *roomln);
    *foundln = *foundln + 1;
    if(atlfound == 1)
    {
        fileio_execuntilend(*foundln);
    }

    free(foundln);
}

void room_load(char* id)
{
    int roomln = 0;

    pvars_setstdvars("currentroom", id);
    find_roomline(id, &roomln);
    room_atlaunch(&roomln);
}

