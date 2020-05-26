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
}

#include <cstdlib>
#include <cstring>
#include "init.h"
#include "fileio/gameconf.hpp"
#include "room/room_io.h"
#include "room/room.hpp"
#include "vars/pvars.hpp"
#include "cutscenes.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "userio.h"

#include <array>
#include <string>

namespace init
{
    struct lang_item
    {
        lang_item(std::string p_id, std::string p_disp) : id(p_id),
            disp(p_disp) {}
            
        std::string id;
        std::string disp;
    };

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
        pvars::setstdvars("csfile", pvars::getgcvars("csfile"));
        pvars::setstdvars("lang", defaultlang);
        pvars::setstdvars("roomfile", roomfile);
    }

    static void show_lang_prompt(std::array<lang_item, 2> p_arr)
    {
        std::string hint_str(
            "Hint : make a choice by typing the corresponding number.");

        move(pcurses::title_y, 0);
        pcurses::display_center_string(hint_str);
        printw("\n\n\n");
        pcurses::display_center_string("Select your language:");
        printw("\n\n");

        for(unsigned int i = 1; i <= p_arr.size(); ++i) {
            std::string disp_str(std::to_string(i));

            disp_str += ". ";
            disp_str.append(p_arr[i-1].disp);
            move(getcury(stdscr), 0);
            pcurses::display_pos_string(disp_str, pcurses::choice_space);
            move(getcury(stdscr) + 1, pcurses::margin);
        }

        move(getcury(stdscr) + 1, pcurses::margin);
        pcurses::display_pos_string("Your choice: ", 12);
        refresh();
    }

    //Show a prompt asking the user to choose the language
    static void ask_lang()
    {
        bool validinp = false;
        std::array<lang_item, 2> langarr {
            lang_item("en", "English"),
            lang_item("fr", "Français")
        };

        clear();

        while(!validinp) {
            std::string buf;

            show_lang_prompt(langarr);
            refresh();
            buf = userio::gettextinput(2);

            if(buf.size() == 1) {
                int intval = buf[0] - '0';
                
                if(intval > 0 && intval <= static_cast<int>(langarr.size())) {
                    std::string lang = langarr[intval - 1].id;

                    pvars::setstdvars("lang", lang.c_str());
                    validinp = true;
                    pstrings::copy_file_to_vec();
                } else {
                    clear();
                    move(LINES - 3, pcurses::margin);
                    attron(A_BOLD);
                    printw("Nope. (not a valid input)\n");
                    attroff(A_BOLD);
                    refresh();
                }
            } else if(buf.size() == 0) {
                clear();
                attron(A_BOLD);
                move(LINES - 3, pcurses::margin);
                printw("Nope. (nothing !)\n");
                attroff(A_BOLD);
            } else {
                clear();
                attron(A_BOLD);
                move(LINES - 3, pcurses::margin);
                printw("Nope. (too long)\n");
                attroff(A_BOLD);
            }
        }
    }
}

void init_game()
{
    std::string room_name; 

    init::set_curses();
    init::set_pvars(room_name);
    roomio::copy_file_to_vec();
    init::ask_lang();
    cutscenes::copy_file_to_vec();
    roommod::start_loop(room_name);
}
