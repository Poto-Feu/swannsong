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
    int title_y = 0;
    unsigned int lines = 0;
    unsigned int cols = 0;

    unsigned int max_size_str()
    {
        return COLS - margin * 2;
    }

    int find_centered_x(std::string& p_str)
    {
        return COLS / 2 - static_cast<int>(p_str.size()) / 2;
    }

    void disp_str(std::string& str, int p_x)
    {
        move(getcury(stdscr), p_x);
        printw("%s", str.c_str());
    }

    void display_pos_string(std::string p_str, unsigned int x_space)
    {
        const unsigned int multiline_space = 3;

        bool end_of_str = false;
        bool end_of_zone = false;
        unsigned int cur_y = getcury(stdscr);
        unsigned int pos_str_max_size = max_size_str() / 2 + x_space;
        unsigned int str_size = p_str.size();

        for(auto i = 0; !end_of_str && !end_of_zone; ++i)
        {
            str_size = p_str.size();

            if(cur_y > lines - 5) end_of_zone = true;
            else if(str_size > pos_str_max_size)
            {
                std::string curr_str = p_str.substr(0, pos_str_max_size);

                p_str.erase(0, pos_str_max_size);
                disp_str(curr_str, cols / 2 - x_space);
                printw("\n");
            } else
            {
                disp_str(p_str, cols / 2 - x_space);
                end_of_str = true;
            }

            if(i == 0)
            {
                x_space += multiline_space;
                pos_str_max_size -= multiline_space;
            }

            ++cur_y;
        }
    }

    void display_center_string(std::string p_str, int space)
    {
        bool end_of_str = false;
        bool end_of_zone = false;
        unsigned int str_size = p_str.size();

        while(!end_of_str && !end_of_zone)
        {
            unsigned int cy = getcury(stdscr);

            if(cy > lines - 5) end_of_zone = true;
            else if(str_size > max_size_str())
            {
                std::string curr_str = p_str.substr(0, max_size_str());

                move(getcury(stdscr), margin);
                p_str.erase(0, max_size_str());
                printw("%s\n", curr_str.c_str());
            } else
            {
                int x = find_centered_x(p_str);

                move(getcury(stdscr), x + space);
                printw("%s", p_str.c_str());

                end_of_str = true;
            }
        }
    }
}
