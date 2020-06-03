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
#include <curses.h>
#include "perror.h"
}

#include <string>
#include "Choice.hpp"
#include "room_io.h"
#include "interpreter/parser.hpp"
#include "cutscenes.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "stringsm.h"

Choice::Choice(int ch_n, int ch_ln) : choice_n(ch_n), choice_line(ch_ln) { }

void Choice::display() const
{
    bool textfound = false;
    int currln = choice_line + 1;

    for(int i = 0; !textfound; i++) {
        int x = 0;
        int y = 0;
        std::string buf;
        std::string type;
        std::string arg;
        std::string disp_value;

        getyx(stdscr, y, x);
        roomio::fetch_ln(buf, currln);
        parser::splitline(type, arg, buf);

        if(type == "TEXT") {
            textfound = true;

            if(stringsm::is_str(arg)) {
                disp_value = stringsm::ext_str_quotes(arg);
            } else {
                disp_value = pstrings::fetch(arg);
                disp_value.insert(0, ". ");
                disp_value.insert(0, std::to_string(choice_n));
            }

            move(y, 0);
            pcurses::display_pos_string(disp_value, pcurses::choice_space);
            printw("\n");

            currln++;
        }
        else if(type == "END") perror_disp("missing choice text", true);
    }
}

unsigned int Choice::getLine() const
{
    return choice_line;
}
