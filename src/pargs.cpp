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

#include <cstring>
#include "pargs.hpp"
#include "files_path.hpp"
#include "game_error.hpp"

namespace pargs
{
    void init(int const argc, char* argv[])
    {
        auto has_arg = [=](const char* p_arg)
        {
            for(int i = 1; i < argc; ++i) {
                if(!strcmp(p_arg, argv[i])) return true;
            } return false;
        };

        if(has_arg("-local")) files_path::setlocal();
        if(has_arg("-debug")) game_error::setdebug();
    }
}
