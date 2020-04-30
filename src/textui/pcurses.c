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

#include <curses.h>
#include "pcurses.h"

void pcurses_init()
{
    initscr();
    raw();
    noecho();
}

void pcurses_end()
{
    endwin();
}

void pcurses_refresh()
{
    refresh();
}

void pcurses_clear()
{
    clear();
}

void pcurses_echo()
{
    echo();
}

void pcurses_noecho()
{
    noecho();
}

void pcurses_display_text(const char* p_str)
{
    printw(p_str);
}

void pcurses_getnstr(char* p_str, int n)
{
    getnstr(p_str, n);
}

int pcurses_getch()
{
    return getch();
}
