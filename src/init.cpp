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
#include "vars/pconst.h"
}

#include <cstdlib>
#include <cstring>
#include "init.h"
#include "fileio/gameconf.hpp"
#include "room/room_io.h"
#include "room/room.h"
#include "vars/pvars.h"
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

    void set_title_y()
    {
        pcurses::title_y = LINES / 2 - LINES / 6;
    }

    void set_coord()
    {
        pcurses::lines = LINES;
        pcurses::cols = COLS;
    }

    void set_curses()
    {
        initscr();
        raw();
        noecho();

        set_margin();
        set_title_y();
        set_coord();
    }

    void set_pvars(std::string& room_name)
    {
        std::string defaultlang;
        std::string roomfile;

        gameconf::readfile();
        defaultlang = pvars::getgcvars("defaultlang");
        room_name = pvars::getgcvars("firstroom");
        roomfile = pvars::getgcvars("roomfile");
        pvars::setstdvars("lang", defaultlang);
        pvars::setstdvars("roomfile", roomfile);
    }

    /*Show a prompt asking the user to choose the language*/
    void ask_lang()
    {
        struct lang_item
        {
            lang_item(std::string p_id, std::string p_disp) : id(p_id),
                disp(p_disp) {}
                
            std::string id;
            std::string disp;
        };

        bool validinp = false;

        std::string hint_str(
            "Hint : make a choice by typing the corresponding number.");

        std::vector<lang_item> langvec
        {
            lang_item("en", "English"),
            lang_item("fr", "Français")
        };

        clear();

        move(pcurses::title_y, 0);
        pcurses::display_center_string(hint_str);
        printw("\n\n\n");
        pcurses::display_center_string("Select your language:");
        printw("\n\n");

        for(int i = 1; i <= static_cast<int>(langvec.size()); ++i)
        {
            std::string disp_str(std::to_string(i));

            disp_str += ". ";
            disp_str.append(langvec[i-1].disp);


            move(getcury(stdscr), 0);
            pcurses::display_pos_string(disp_str, 6);
            printw("\n");
        }

        printw("\n");

        while(!validinp)
        {
            std::string buf;

            pcurses::display_pos_string("Your choice: ", 12);
            refresh();

            buf = userio_gettextinput(2);

            if(buf.size() == 1)
            {
                int intval = buf[0] - '0';
                
                if(intval > 0 && intval <= static_cast<int>(langvec.size()))
                {
                    std::string lang = langvec[intval - 1].id;

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
    std::string room_name; 

    init::set_curses();

    init::set_pvars(room_name);
    roomio_copy_file_to_vec();

    init::ask_lang();
    room_load(room_name.c_str());
}
