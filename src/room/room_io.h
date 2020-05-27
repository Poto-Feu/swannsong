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

#ifndef ROOM_IO_H
#define ROOM_IO_H

#include <string>

bool roomio_find_ind(int& f_ln, std::string p_ln);

namespace roomio
{
    void copy_file_to_vec();
    bool find_ind(int& f_ln, std::string const& p_ln);
    bool fetch_ln(std::string& p_ln, int ind);
}

#endif
