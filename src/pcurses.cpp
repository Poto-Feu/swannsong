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

    static void multiline_center_string(std::string const& p_str)
    {
        bool end_of_str = false;
        std::string remain_str = p_str;
        std::vector<std::string> str_vec;

        while(!end_of_str)
        {
            std::string vec_item;

            if(remain_str.size() > max_size_str()) {
                vec_item = remain_str.substr(0, max_size_str());
                remain_str.erase(0, max_size_str());
                str_vec.push_back(vec_item);
            } else {
                end_of_str = true;
            }

        }

        for(auto const& vec_it : str_vec)
        {
            move(getcury(stdscr), pcurses::margin);
            printw("%s\n", vec_it.c_str());
        }

        move(getcury(stdscr), pcurses::margin);
        printw("%s", remain_str.c_str());
    }

    int find_centered_x(std::string const& p_str)
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

    void display_center_string(std::string const& p_str)
    {
        unsigned int max_size_func = max_size_str();

        if(p_str.size() < max_size_func) {
            unsigned int p_x = find_centered_x(p_str);

            move(getcury(stdscr), p_x);
            printw("%s", p_str.c_str());
        } else {
            multiline_center_string(p_str);
        }
    }
}
