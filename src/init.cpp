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
#include "perror.h"
}

#include <algorithm>
#include <array>
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
    static void init_game(std::string& room_name)
    {
        struct lang_item
        {
            lang_item(std::string const& p_id, std::string const& p_disp) :
                id(p_id), disp(p_disp) {}

            std::string id;
            std::string disp;
        };

        auto set_curses = []()
        {
            auto set_margin = []()
            {
                if(COLS < 100) pcurses::margin = 4;
                else if(COLS > 200) pcurses::margin = 15;
                else pcurses::margin = 10;
            };

            auto set_title_y = []()
            {
                pcurses::title_y = LINES / 2 - LINES / 6;
            };

            auto set_coord = []()
            {
                pcurses::lines = LINES;
                pcurses::cols = COLS;
            };

            initscr();
            raw();
            noecho();

            set_margin();
            set_title_y();
            set_coord();
        };

        auto missing_gcvar = [](std::string const& p_name)
        {
            std::string err_msg = "missing gameconf var (" + p_name +")";
            perror_disp(err_msg.c_str(), true);
        };

        //Fetch variables from the gameconf file and return a vector containing them
        auto fetch_gameconf_vars = [&]()
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
        };

        //Show a prompt asking the user to choose the language
        auto ask_lang = [](std::string const& p_langdir)
        {
            auto show_lang_prompt = [](std::array<lang_item, 2> p_arr)
            {
                int str_line = pcurses::title_y;
                std::string hint_str(
                    "Hint : make a choice by typing the corresponding number.");

                pcurses::display_center_string(hint_str, str_line);
                str_line = display_server::get_last_line() + 3;
                pcurses::display_center_string("Select your language:", str_line);
                str_line = display_server::get_last_line() + 2;

                for(unsigned int i = 1; i <= p_arr.size(); ++i) {
                    std::string disp_str(std::to_string(i));

                    disp_str += ". ";
                    disp_str.append(p_arr[i-1].disp);
                    pcurses::display_pos_string(disp_str, pcurses::choice_space, str_line);
                    str_line = display_server::get_last_line() + 1;
                }

                str_line += 1;
                pcurses::display_pos_string("Your choice: ", 12, str_line);
                display_server::show_screen();
            };

            bool validinp = false;

            std::array<lang_item, 2> langarr {
                lang_item("eng", "English"),
                lang_item("fra", "Français")
            };

            display_server::clear_screen();
            show_lang_prompt(langarr);
            display_server::save_screen();

            while(!validinp) {
                std::string buf;

                auto show_err_msg = [](std::string const& p_msg)
                {
                    display_server::clear_screen();
                    display_server::add_string(p_msg, {LINES - 3, pcurses::margin}, A_BOLD);
                    display_server::load_save();
                    display_server::show_screen();
                };

                buf = userio::gettextinput(2);

                if(buf.size() == 1) {
                    int intval = buf[0] - '0';
                    
                    if(intval > 0 && intval <= static_cast<int>(langarr.size())) {
                        std::string lang = langarr[intval - 1].id;

                        langmod::set_lang(lang);
                        validinp = true;
                        pstrings::copy_file_to_vec(p_langdir);
                    } else show_err_msg("Nope. (not a valid input)");
                } else if(buf.size() == 0) show_err_msg("Nope. (nothing !)");
                else show_err_msg("Nope. (too long)");
            }
        };

        set_curses();

        auto gc_vec = fetch_gameconf_vars();

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

        if(langdir_it != gc_vec.cend()) ask_lang(langdir_it->value);
        else missing_gcvar("langdir");

        if(roomfile_it != gc_vec.cend()) roomio::copy_file_to_vec(roomfile_it->value);
        else missing_gcvar("roomfile");

        if(csfile_it != gc_vec.cend()) cutscenes::copy_file_to_vec(csfile_it->value);
        else missing_gcvar("csfile");

        if(firstroom_it != gc_vec.cend()) room_name = firstroom_it->value;
        else missing_gcvar("firstroom");

    }

    void start_game()
    {
        std::string room_name;

        init_game(room_name);
        roommod::start_loop(room_name);
    }
}
