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

#include <stdarg.h>
#include <curses.h>
#include "textui.h"

void textui_init()
{
    initscr();
    raw();
    noecho();
}

void textui_exit()
{
    endwin();
}

void textui_update()
{
    refresh();
}

void textui_newpage()
{
    clear();
}

void textui_showchr()
{
    echo();
}

void textui_hidechr()
{
    noecho();
}

void textui_move(int y, int x)
{
    move(y, x);
}

void textui_display(const char* p_str, ...)
{
    va_list args;

    va_start(args, p_str);
    vw_printw(stdscr, p_str, args);
}

void textui_readuserinp(char* p_str, int n)
{
    getnstr(p_str, n);
}

int textui_getLINES()
{
    return LINES;
}

int textui_getCOLS()
{
    return COLS;
}

void textui_waitenter()
{
#ifdef _WIN32
    int enter_ch = 13;
#else
    int enter_ch = '\n';
#endif

    while(getch() != enter_ch) {}
}
