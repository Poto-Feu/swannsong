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

extern "C" {
#include <curses.h>
}

#include "exitgame.h"
#include "pstrings.h"
#include "userio.h"
#include "pcurses.hpp"

void exitgame(int c)
{
    move(LINES - 3, pcurses::margin);
    if(c == 0)  pstrings_display("exit_penter");
    else printw("Press Enter to exit");

    refresh();
    userio_waitenter();

    endwin();

    if(c == 0) exit(c);
    else exit(1);
}
