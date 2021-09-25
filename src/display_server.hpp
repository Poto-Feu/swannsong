/*
    Copyright (C) 2021 Adrien Saad

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
    along with SwannSong Adventure. If not, see
    <https://www.gnu.org/licenses/>.
*/

#ifndef DISPLAY_SERVER_HPP
#define DISPLAY_SERVER_HPP

extern "C" {
#include <ncurses.h>
}

#include <string>

namespace display_server
{
    const int NULL_ATTR = 0;
    const int LAST_LINE_ERR = 0;

    struct coord_struct
    {
        int y = getcury(stdscr);
        int x = getcurx(stdscr);
    };

    void add_string(std::string const& p_str,
            coord_struct p_struct = { getcury(stdscr), getcurx(stdscr) },
            int p_attr = NULL_ATTR);
    void clear_screen();
    int get_last_line();
    void show_screen();
}

#endif
