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
    along with SwannSong Adventure.  If not, see
    <https://www.gnu.org/licenses/>.
*/

#include <fstream>
#include <string>
#include <vector>

#include "tests.hpp"
#include "room/interpreter/token.hpp"
#include "pcurses.hpp"
#include "stringsm.hpp"

namespace tests
{
    void stringsm_is_str()
    {
        std::string test_str("\"SwannSong Adventure\"");
        if(stringsm::is_str(test_str)) printf("true\n");
        else printf("false\n");

        exit(0);
    }

    void token(pstrings::ps_data_ptr const& pstrings_data)
    {
        std::string test_str("TITLE SwannSong Adventure");
        TokenVec test_vec = token::create_arr(test_str, pstrings_data);

        for(auto& it : test_vec) {
            printf("type: %d\nvalue: %s\n\n", static_cast<int>(it.type),
                    it.str.c_str());
        }

        exit(0);
    }
}
