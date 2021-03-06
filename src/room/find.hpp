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


#ifndef FIND_H
#define FIND_H

#include <string>

namespace room_find
{
    bool room_property(std::string& value, std::string const& prop, int roomln);
    bool onechoiceline(int num, int startln, int& ln);
    bool blockline(int& foundln, int p_ln, std::string const& ins);
    bool atlaunchline(int& foundln, int ln);
    bool choicesline(int& foundln, int room_ln);
    bool roomline(int* r_ln, std::string const& id);
    int roomline(std::string const& id);
}
#endif
