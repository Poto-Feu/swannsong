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

#ifndef PCURSES_H
#define PCURSES_H

#include <string>
#include <vector>

struct pcur_str
{
    pcur_str(std::string p_str) : str(p_str) { }

    int x;
    std::string str;
};

struct pcur_struct
{
    pcur_struct(std::string p_str, int p_y) : starty(p_y), full_str(p_str) { }

    int starty;
    std::string full_str;
    std::vector<pcur_str> vec;
};

namespace pcurses
{
    extern int margin;

    int find_centered_x(std::string& p_str);
    void display_string(std::string p_str, int p_y);
}

#endif
