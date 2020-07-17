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

#include <iostream>
#include "files_path.hpp"
#include "game_error.hpp"
#include "exitgame.h"
#include "os_module.hpp"

namespace files_path
{
    using namespace os_module;
    namespace fs = std::filesystem;

    //Indicate if the program is run from the build folder or if it is installed on the user machine
    static bool runned_locally = false;

    bool create_directory(fs::path const& p_path)
    {
        if(!fs::exists(p_path)) {
            fs::create_directory(p_path);
            return true;
        } else return false;
    }

    static fs::path init_data_path()
    {
#ifdef IS_PKG
        return "/usr/share/swannsong_adventure/";
#else
        if(current_os == os_type::WINDOWSNT || runned_locally) {
            return "data/";
        } else return "/usr/local/share/swannsong_adventure/";
#endif
    }
            
    static fs::path init_user_data_path(paths_struct& rtrn_struct)
    {
        fs::path path_fs;

        if(current_os == os_type::UNIXLIKE) {
            path_fs = getenv("HOME");
            path_fs += "/.local";
            files_path::create_directory(path_fs);
            path_fs += "/share";
            files_path::create_directory(path_fs);
            path_fs += "/swannsong_adventure/";
            files_path::create_directory(path_fs);
            rtrn_struct.local_data_path = path_fs;
        } else if(current_os == os_type::WINDOWSNT) {
            path_fs = getenv("LOCALAPPDATA");

            path_fs += "/swannsong_adventure/";
            files_path::create_directory(path_fs);
            rtrn_struct.local_data_path = path_fs;
        } else {
            std::cout << "Please use a POSIX or a WINDOWS NT environnement to run this program"
                << "\n";
            exit(1);
        }
        return path_fs;
    }

    //Return a struct containing the system game data path and the user-specific game data path
    paths_struct getpaths()
    {
        paths_struct rtrn_struct;

        rtrn_struct.data_path = init_data_path();
        rtrn_struct.local_data_path = init_user_data_path(rtrn_struct);
        return rtrn_struct;
    }

    void setlocal()
    {
        runned_locally = true;
    }
}
