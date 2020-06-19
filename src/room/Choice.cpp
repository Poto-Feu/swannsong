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

#include <string>
#include "room/Choice.hpp"
#include "room/room_io.h"
#include "interpreter/parser.hpp"
#include "game_error.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "stringsm.h"

Choice::Choice(int ch_n, int ch_ln) : choice_n(ch_n), choice_line(ch_ln) { }

void Choice::display() const
{
    bool textfound = false;
    int currln = choice_line + 1;

    for(int i = 0; !textfound; i++) {
        std::string buf;
        std::string f_type;
        std::string f_arg;
        std::string disp_value;

        roomio::fetch_ln(buf, currln);
        parser::splitline(f_type, f_arg, buf);

        if(f_type == "TEXT") {
            int str_line = display_server::get_last_line() + 1;
            textfound = true;

            if(stringsm::is_str(f_arg)) disp_value = stringsm::ext_str_quotes(f_arg);
            else {
                disp_value = pstrings::fetch(f_arg);
                disp_value.insert(0, ". ");
                disp_value.insert(0, std::to_string(choice_n));
            }

            if(str_line == display_server::LAST_LINE_ERR + 1) str_line = pcurses::title_y + 4;
            pcurses::display_pos_string(disp_value, pcurses::choice_space, str_line);
            currln++;
        } else if(f_type == "END") game_error::fatal_error("missing choice text");
    }
}

unsigned int Choice::getLine() const
{
    return choice_line;
}
