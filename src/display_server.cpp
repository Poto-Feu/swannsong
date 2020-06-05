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

#include <array>
#include <vector>
#include "display_server.hpp"

namespace display_server
{
    static const int arr_y = 0;
    static const int arr_x = 1;

    struct displayed_sting
    {
        std::string str;
        int attribute = NULL_ATTR;
        std::array<int, 2> coord;
    };

    static std::vector<displayed_sting> current_screen;

    //This vector act as a kind of temporary location to store a reusable screen
    static std::vector<displayed_sting> saved_screen;

    void add_string(std::string const& p_str, coord_struct p_struct, int p_attr)
    {
        std::array<int, 2> p_coord { p_struct.y, p_struct.x };
        current_screen.push_back(displayed_sting{p_str, p_attr, p_coord});
    }

    void clear_screen()
    {
        current_screen.clear();
    }

    int get_last_line()
    {
        if(!current_screen.empty()) return current_screen[current_screen.size() - 1].coord[arr_y];
        else return LAST_LINE_ERR;
    }

    void load_save()
    {
        for(auto const& it : saved_screen) current_screen.push_back(it);
    }

    void save_screen()
    {
        saved_screen.clear();
        saved_screen = current_screen;
    }

    void show_screen()
    {
        clear();
        for(auto const& it : current_screen) {
            if(it.attribute != NULL_ATTR) attron(it.attribute);
            move(it.coord[arr_y], it.coord[arr_x]);
            printw(it.str.c_str());
            if(it.attribute != NULL_ATTR) attroff(it.attribute);
        }
        refresh();
    }
}
