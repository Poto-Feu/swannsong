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

#include <cstdio>
#include "tests.hpp"
#include "room/interpreter/token.hpp"
#include "exitgame.h"
#include "stringsm.h"

#include <string>
#include <vector>

namespace tests
{
    void stringsm()
    {
        std::string test_str("\"SwannSong Adventure\"");
        if(stringsm::is_str(test_str)) printf("true\n");
        else printf("false\n");

        exitgame(0);
    }

    void token()
    {
        std::string test_str("TITLE SwannSong Adventure");
        TokenVec test_vec = token::create_arr(test_str);

        for(auto& it : test_vec)
        {
            printf("type: %d\nvalue: %s\n\n", static_cast<int>(it.type),
                    it.str.c_str());
        }
        exitgame(0);
    }
}
