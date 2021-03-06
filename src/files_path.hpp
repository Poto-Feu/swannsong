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

#ifndef FILES_PATH_HPP
#define FILES_PATH_HPP

#include <filesystem>
#include <string>

namespace files_path
{
    struct paths_struct {
        std::filesystem::path data_path;
        std::filesystem::path local_data_path;
    };

    bool create_directory(std::filesystem::path const& p_path);
    paths_struct getpaths();
    void setlocal();
}

#endif
