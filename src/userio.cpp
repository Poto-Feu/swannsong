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

extern "C" {
#include <curses.h>
}

#include <cstdlib>
#include "userio.hpp"
#include "stringsm.h"

/*Get user text input and return it in a pointer*/
static void copy_input_to_char_str(char** buf, int max_n)
{
    *buf = (char*)calloc(max_n+1, sizeof(char));

    echo();
    getnstr(*buf, max_n);
    noecho();

    stringsm_chomp(*buf);
}

namespace userio
{
    /*Pause the program until the user press Enter*/
    void waitenter()
    {
        #ifdef _WIN32
        int enter_ch = WIN_ENTER_KEY;
        #else
        int enter_ch = '\n';
        #endif

        while(getch() != enter_ch) {}
    }

    std::string gettextinput(int max_n)
    {
        char* buf = NULL;
        std::string r_str;

        copy_input_to_char_str(&buf, max_n);
        r_str.assign(buf);

        free(buf);

        return r_str;
    }
}
