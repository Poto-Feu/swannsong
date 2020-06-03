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

#include <algorithm>
#include <array>
#include <string>
#include "init.hpp"
#include "fileio/gameconf.hpp"
#include "room/room_io.h"
#include "room/room.hpp"
#include "cutscenes.hpp"
#include "lang.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "userio.h"

namespace init
{
    static void missing_gcvar(std::string const& p_name)
    {
        std::string err_msg = "missing gameconf var (" + p_name +")";
        perror_disp(err_msg.c_str(), true);
    }

    struct lang_item
    {
        lang_item(std::string const& p_id, std::string const& p_disp) :
            id(p_id), disp(p_disp) {}

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

    /*Fetch variables from the gameconf file and return a vector containing them*/
    std::vector<gameconf::gcvar_struct> fetch_gameconf_vars()
    {
        auto gc_vec = gameconf::readfile();

        auto get_gcvar_it = [gc_vec](std::string const& p_name) {
            return std::find_if(gc_vec.cbegin(), gc_vec.cend(),
                [p_name](gameconf::gcvar_struct const& cgcvar) {
                return cgcvar.name == p_name;
                });
        };

        auto defaultlang_it = get_gcvar_it("defaultlang");
        auto langdir_it = get_gcvar_it("langdir");

        if(defaultlang_it != gc_vec.cend()) {
            langmod::set_lang(defaultlang_it->value);
        } else missing_gcvar("defaultlang");

        if(langdir_it != gc_vec.cend()) {
            langmod::set_langdir(langdir_it->value);
        } else missing_gcvar("langdir");

        return gc_vec;
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
    static void ask_lang(std::string const& p_langdir)
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

                    langmod::set_lang(lang);
                    validinp = true;
                    pstrings::copy_file_to_vec(p_langdir);
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

    static void init_game(std::string& room_name)
    {
        init::set_curses();

        auto gc_vec = init::fetch_gameconf_vars();

        auto get_gcvar_it = [gc_vec](std::string const& p_name) {
            return std::find_if(gc_vec.cbegin(), gc_vec.cend(),
                [p_name](gameconf::gcvar_struct const& ccpvar) {
                return ccpvar.name == p_name;
                });
        };

        auto langdir_it = get_gcvar_it("langdir");
        auto roomfile_it = get_gcvar_it("roomfile");
        auto csfile_it = get_gcvar_it("csfile");
        auto firstroom_it = get_gcvar_it("firstroom");

        if(langdir_it != gc_vec.cend()) init::ask_lang(langdir_it->value);
        else init::missing_gcvar("langdir");

        if(roomfile_it != gc_vec.cend()) roomio::copy_file_to_vec(roomfile_it->value);
        else init::missing_gcvar("roomfile");

        if(csfile_it != gc_vec.cend()) cutscenes::copy_file_to_vec(csfile_it->value);
        else init::missing_gcvar("csfile");

        if(firstroom_it != gc_vec.cend()) room_name = firstroom_it->value;
        else init::missing_gcvar("firstroom");

    }

    void start_game()
    {
        std::string room_name;

        init_game(room_name);
        roommod::start_loop(room_name);
    }
}
