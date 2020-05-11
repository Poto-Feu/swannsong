/*
    Copyright (C) 2020 Adrien Saad

    This file is part of SwannSong.

    SwannSong is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SwannSong is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong.  If not, see <https://www.gnu.org/licenses/>.
*/

extern "C" {
#include <curses.h>
#include "fileio/gameconf.h"
#include "vars/pvars.h"
}

#include <cstdlib>
#include <cstring>
#include "init.h"
#include "room/room_io.h"
#include "room/room.h"
#include "pcurses.hpp"
#include "pstrings.h"
#include "userio.h"

#include <string>
#include <vector>

namespace init
{
    void set_margin()
    {
        if(COLS < 100) pcurses::margin = 4;
        else if(COLS > 200) pcurses::margin = 15;
        else pcurses::margin = 10;
    }

    void set_curses()
    {
        initscr();
        raw();
        noecho();

        set_margin();
    }

    void set_pvars(char** room_name)
    {
        char* defaultlang = NULL;
        char* roomfile = NULL;

        gameconf_readfile();
        pvars_getgcvars("defaultlang", &defaultlang);
        pvars_getgcvars("firstroom", room_name);
        pvars_getgcvars("roomfile", &roomfile);
        pvars_setstdvars("lang", defaultlang);
        pvars_setstdvars("roomfile", roomfile);
        pvars_freegcvar("defaultlang");
        pvars_freegcvar("firstroom");
        pvars_freegcvar("roomfile");

        free(defaultlang);
        free(roomfile);
    }

    /*Show a prompt asking the user to choose the language*/
    void ask_lang()
    {
        bool validinp = false;
        std::vector<std::string> langarr {"en", "fr"};

        clear();
        printw("Hint : make a choice by typing the corresponding number.\n");
        printw("\nSelect your language:"
                "\n1. English"
                "\n2. Français\n");

        while(!validinp)
        {
            std::string buf;

            printw("\nYour choice: ");
            refresh();

            buf = userio_gettextinput(2);

            if(buf.size() == 1)
            {
                int intval = buf[0] - '0';
                
                if(intval > 0 && intval <= static_cast<int>(langarr.size()))
                {
                    std::string lang = langarr[intval - 1];

                    pvars_setstdvars("lang", lang.c_str());
                    validinp = true;
                    pstrings_copy_file_to_vec();
                } else
                {
                    printw("Nope. (not a valid input)\n");
                    refresh();
                }
            } else if(buf.size() == 0)
            {
                printw("Nope. (nothing !)\n");
                refresh();
            } else
            {
                printw("Nope. (too long)\n");
                refresh();
            }
        }
    }
}

void init_game()
{
    char* room_name = NULL; 

    init::set_curses();

    init::set_pvars(&room_name);
    roomio_copy_file_to_vec();

    init::ask_lang();
    room_load(room_name);

    free(room_name);
}

