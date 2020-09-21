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

#include <fstream>
#include <string>
#include <vector>

#include "tests.hpp"
#include "room/interpreter/token.hpp"
#include "fileio/save/save_const.hpp"
#include "fileio/save/SaveChunk.hpp"
#include "cutscenes.hpp"
#include "exitgame.h"
#include "pcurses.hpp"
#include "stringsm.h"

namespace tests
{
    void stringsm_is_str()
    {
        std::string test_str("\"SwannSong Adventure\"");
        if(stringsm::is_str(test_str)) printf("true\n");
        else printf("false\n");

        exit(0);
    }

    void cutscenes_test()
    {
        cutscenes::display("about");

        exit(0);
    }

    void token()
    {
        std::string test_str("TITLE SwannSong Adventure");
        TokenVec test_vec = token::create_arr(test_str);

        for(auto& it : test_vec) {
            printf("type: %d\nvalue: %s\n\n", static_cast<int>(it.type),
                    it.str.c_str());
        }

        exit(0);
    }

    void savechunk()
    {
        using namespace save_const;

        uint32_t test_val = 12;
        SaveChunk val_chk(save_const::chunk_type::GAMNAME, test_val);
        auto val_chk_vec = val_chk.getChunkAsVector();

        std::ofstream test_stream("test_bin", std::ios::binary);
        test_stream.exceptions(std::ios::badbit | std::ios::failbit);
        test_stream.write((const char*)val_chk_vec.data(), val_chk_vec.size());
        test_stream.flush();

        exit(0);
    }
}
