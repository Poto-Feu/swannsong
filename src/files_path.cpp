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

#include "files_path.hpp"
#include "exitgame.h"
#include "os_module.hpp"

namespace files_path
{
    std::string getdatapath()
    {
        using namespace os_module;

        if(current_os == os_type::UNIXLIKE) return "../share/swannsong_adventure/";
        else if(current_os == os_type::WINDOWSNT) return "data/";
        else exitgame(1);
    }
}
