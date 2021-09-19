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
    along with SwannSong Adventure.
    If not, see <https://www.gnu.org/licenses/>.
*/

#include <vector>

#include "display_server.hpp"

struct displayed_sting
{
    std::string str;
    int attribute = display_server::NULL_ATTR;
    int y;
    int x;
};

static std::vector<displayed_sting> current_screen;

void display_server::add_string(std::string const& p_str,
        coord_struct p_struct, int p_attr)
{
    current_screen.push_back(displayed_sting{p_str, p_attr, p_struct.y,
            p_struct.x});
}

void display_server::clear_screen()
{
    current_screen.clear();
}

int display_server::get_last_line()
{
    if(!current_screen.empty()) {
        return current_screen[current_screen.size() - 1].y;
    } else {
        return LAST_LINE_ERR;
    }
}

void display_server::show_screen()
{
    clear();
    for(auto const& it : current_screen) {
        if(it.attribute != NULL_ATTR) {
            attron(it.attribute);
        }
        move(it.y, it.x);
        printw(it.str.c_str());
        if(it.attribute != NULL_ATTR) {
            attroff(it.attribute);
        }
    }
    refresh();
}
