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
#include "perror.h"
}

#include <iostream>
#include "files_path.hpp"
#include "exitgame.h"
#include "os_module.hpp"

namespace files_path
{
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

    paths_struct getpaths()
    {
        paths_struct rtrn_struct;
        static bool already_used = false;

        if(!already_used) {
            using namespace os_module;
            
            auto init_data_path = [&]() {
#ifdef IS_PKG
                rtrn_struct.data_path = "/usr/share/swannsong_adventure/";
#else
                if(current_os == os_type::WINDOWSNT || runned_locally) {
                    rtrn_struct.data_path = "data/";
                } else rtrn_struct.data_path = "/usr/local/share/swannsong_adventure/";
#endif
            };
            
            auto init_user_data_path = [&]() {
                if(current_os == os_type::UNIXLIKE) {
                    fs::path path_fs = getenv("HOME");

                    path_fs += "/.local";
                    files_path::create_directory(path_fs);
                    path_fs += "/share";
                    files_path::create_directory(path_fs);
                    path_fs += "/swannsong_adventure";
                    files_path::create_directory(path_fs);
                    rtrn_struct.local_data_path = path_fs;
                    rtrn_struct.local_data_path += "/";
                } else if(current_os == os_type::WINDOWSNT) {
                    fs::path path_fs = getenv("LOCALAPPDATA");

                    path_fs += "/swannsong_adventure";
                    files_path::create_directory(path_fs);
                    rtrn_struct.local_data_path = path_fs;
                    rtrn_struct.local_data_path += "/";
                } else {
                    std::cout
                        << "Please use a POSIX or a WINDOWS NT environnement to run this program"
                        << std::endl;
                    exit(1);
                }
            };

            init_data_path();
            init_user_data_path();

            already_used = true;

        } else perror_disp("files_path::initpaths used more than once", true);
        return rtrn_struct;
    }

    void setlocal()
    {
        runned_locally = true;
    }
}
