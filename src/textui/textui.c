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

#include "textui.h"
#include "pcurses.h"

void textui_init()
{
    pcurses_init();
}

void textui_exit()
{
    pcurses_end();
}

void textui_update()
{
    pcurses_refresh();
}

void textui_newpage()
{
    pcurses_clear();
}

void textui_showchr()
{
    pcurses_echo();
}

void textui_hidechr()
{
    pcurses_noecho();
}

void textui_display(const char* p_str)
{
    pcurses_display_text(p_str);
}

void textui_readuserinp(char* p_str, int n)
{
    pcurses_getnstr(p_str, n);
}

void textui_waitenter()
{
#ifdef _WIN32
    int enter_ch = 13;
#else
    int enter_ch = '\n';
#endif

    while(pcurses_getch() != enter_ch) {}
}
