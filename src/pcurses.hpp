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

#ifndef PCURSES_H
#define PCURSES_H

extern "C" {
#include <curses.h>
}

#include <string>
#include <vector>
#include "display_server.hpp"

namespace pcurses
{
    const int top_margin = 3;
    const int choice_space = 8;

    extern int margin;
    extern int title_y;
    extern int lines;
    extern int cols;

    int find_centered_x(std::string& p_str);
    void display_pos_string(std::string p_str, int x_space, int startline = getcury(stdscr),
            int p_attr = display_server::NULL_ATTR);
    void display_center_string(std::string const& p_str, int startline = getcury(stdscr),
            int p_attr = display_server::NULL_ATTR);
}

#endif
