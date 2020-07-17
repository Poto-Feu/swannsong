/*
    Copyright (C) 2020 Adrien Saad

    This file is part of SwannSong Adventure.

    SwannSong Adventure is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SwannSong Adventure is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong Adventure.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "exitgame.h"
#include "pstrings.h"
#include "userio.h"
#include "pcurses.hpp"
#include "display_server.hpp"

void exitgame(int exit_code)
{
    display_server::coord_struct exit_struct {pcurses::lines - 3, pcurses::margin};

    if(exit_code == 0) display_server::add_string(pstrings::fetch("exit_penter"), exit_struct, A_BOLD);
    else display_server::add_string("Press Enter to exit", exit_struct, A_BOLD);

    display_server::show_screen();
    userio::waitenter();
    direct_exit(exit_code);
}

void direct_exit(int exit_code)
{
    delwin(stdscr);
    endwin();
    exit(exit_code);
}
