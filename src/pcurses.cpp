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

#include <string>
#include <vector>
#include "pcurses.hpp"

namespace pcurses
{
    int margin = 0;
    int max_size_str()
    {
        return COLS - margin * 2;
    }

    int find_centered_x(std::string& p_str)
    {
        return COLS / 2 - static_cast<int>(p_str.size()) / 2;
    }

    void display_string(std::string p_str, int p_y, int space, bool newline)
    {
        bool end_of_str = false;

        while(!end_of_str)
        {
            int str_size = static_cast<int>(p_str.size());

            if(str_size > max_size_str())
            {
                std::string curr_str = p_str.substr(0, max_size_str());

                move(p_y, margin);
                p_str.erase(0, max_size_str());
                printw("%s\n", curr_str.c_str());
                
                ++p_y;
            } else
            {
                int x = find_centered_x(p_str);

                move(p_y, x + space);
                printw("%s", p_str.c_str());
                if(newline) printw("\n");

                end_of_str = true;
            }
        }
    }
}
