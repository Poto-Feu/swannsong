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
    along with SwannSong Adventure.  If not, see
    <https://www.gnu.org/licenses/>.
*/

#ifndef PCURSES_HPP
#define PCURSES_HPP

#include <vector>

#include "display_server.hpp"
#include "pstrings.hpp"

namespace pcurses
{
    const int top_margin = 3;
    const int bottom_margin = top_margin;

    extern int margin;
    extern int title_y;
    extern int lines;
    extern int cols;

    /*Return a vector in which the string has been cut into multiple string to
     * fit the terminal width */
    std::vector<std::string> divide_string_into_lines(std::string p_string);
    void display_pos_string(std::string p_str, int x_space,
            int startline = getcury(stdscr),
            int p_attr = display_server::NULL_ATTR);
    unsigned int display_center_string(std::string const& p_str,
            int startline = getcury(stdscr),
            int p_attr = display_server::NULL_ATTR);
    void display_penter_message(pstrings::ps_data_ptr const& pstrings_data,
            bool wait_enter = true);
    std::string get_text_input(int max_n);
}
#endif
