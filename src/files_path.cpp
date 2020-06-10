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

#include "files_path.hpp"
#include "exitgame.h"
#include "os_module.hpp"

namespace files_path
{
    //Indicate if the program is run from the build folder or if it is installed on the user machine
    static bool runned_locally = false;
    static std::string data_path;

    void initpaths()
    {
        static bool already_used = false;
        if(!already_used) {
            using namespace os_module;

#ifdef IS_PKG
            data_path = "/usr/share/swannsong_adventure/";
#else
            if(current_os == os_type::WINDOWSNT || runned_locally) data_path = "data/";
            else data_path = "/usr/local/share/swannsong_adventure/";
#endif

            already_used = true;
        } else perror_disp(
                "files_path::initpaths used more than once (this might cause problems)",
                false);
    }

    void setlocal()
    {
        runned_locally = true;
    }

    std::string getdatapath()
    {
        return data_path;
    }
}
