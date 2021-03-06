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

#include <vector>
#include <fstream>
#include "room_io.h"
#include "fileio/fileio.h"

namespace roomio
{
    static std::vector<std::string> file_vec;

    //Copy room file lines into a vector
    void copy_file_to_vec(std::string const& roomfile, std::filesystem::path const& data_path)
    {
        file_vec = fileio::copy_to_vector(data_path.string() + roomfile);
    }

    //Return the line number where the specified line is present
    bool find_ind(int& f_ln, std::string const& p_ln)
    {
        int i = 1;

        for(const auto& it : roomio::file_vec) {
            if(it == p_ln) {
                f_ln = i;
                return true;
            } else ++i;  
        }
        f_ln = 0;

        return false;
    }

    //Return a char array containing the line from the specified index
    bool fetch_ln(std::string& p_ln, int ind)
    {
        int real_ind = ind - 1;

        if(ind > static_cast<int>(roomio::file_vec.size())) return false;
        else {
            p_ln = roomio::file_vec[real_ind];
            return true;
        }
    }
}
