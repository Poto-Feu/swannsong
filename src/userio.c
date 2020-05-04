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

#include <stdlib.h>
#include <curses.h>
#include "stringsm.h"

#define WIN_ENTER_KEY 13

/*Pause the program until the user press Enter*/
void userio_waitenter()
{
#ifdef _WIN32
    int enter_ch = WIN_ENTER_KEY;
#else
    int enter_ch = '\n';
#endif

    while(getch() != enter_ch) {}
}

/*Get user text input and return it in a pointer*/
void userio_gettextinput(char** buf, int max_n)
{
    *buf = calloc(max_n+1, sizeof(char));

    echo();
    getnstr(*buf, max_n);
    noecho();

    stringsm_chomp(*buf);
}
