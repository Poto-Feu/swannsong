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

#pragma once

#include <filesystem>
#include <string>

namespace files_path
{
    struct paths_struct {
        std::filesystem::path data_path;
        std::filesystem::path local_data_path;
        std::filesystem::path local_conf_path;
    };

    bool create_directory(std::filesystem::path const& p_path);
    /*is_local indicate if the program is run from the build folder or if it is installed on the
    user machine*/
    paths_struct getpaths(bool is_local);
    std::filesystem::path get_local_data_path();
}
