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

#include <fstream>

#include <sys/stat.h>

#include "fileio.h"
#include "stringsm.h"

namespace fileio
{
    bool getfileln(std::string& r_str, std::ifstream& p_stream)
    {
        if(std::getline(p_stream, r_str)) return true;
        else return false;
    }

    std::vector<std::string> copy_to_vector(std::filesystem::path const& file_path)
    {
        std::string buf;
        std::vector<std::string> file_content;
        std::ifstream file_stream(file_path);

        while(fileio::getfileln(buf, file_stream)) {
            stringsm::rtab(buf);

            if(!buf.empty()) file_content.push_back(buf);
            else continue;
        }

        return file_content;
    }

    bool file_exists(std::filesystem::path const& file_path)
    {
        struct stat buffer;
        return (stat(file_path.string().c_str(), &buffer) == 0);
    }
}
