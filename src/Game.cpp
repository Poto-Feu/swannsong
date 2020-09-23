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

#include <algorithm>
#include <array>
#include <cstdio>

#include "Game.hpp"
#include "fileio/gameconf.hpp"
#include "room/RoomManager.hpp"
#include "cutscenes.hpp"
#include "files_path.hpp"
#include "game_error.hpp"
#include "lang.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "userio.h"

const int LANG_NUMBER = 2;

struct lang_item
{
    lang_item(std::string const& p_id, std::string const& p_disp)
        : id(p_id), disp(p_disp) {}

    std::string id;
    std::string disp;
};

void Game::missing_gcvar(std::string const& p_name)
{
    game_error::fatal_error("missing gameconf var (" + p_name +")");
}

static void set_curses()
{
    initscr();
    raw();
    noecho();

    if(COLS < 100) pcurses::margin = 4;
    else if(COLS > 200) pcurses::margin = 15;
    else pcurses::margin = 10;

    pcurses::title_y = LINES / 2 - LINES / 6;
    pcurses::lines = LINES;
    pcurses::cols = COLS;
}

//Fetch variables from the gameconf file and return a vector containing them
auto Game::fetch_gameconf_vars(std::filesystem::path const& system_data_path)
{
    auto gc_vec = gameconf::readfile(system_data_path);

    auto get_gcvar_it = [gc_vec](std::string const& p_name) {
        return std::find_if(gc_vec.cbegin(), gc_vec.cend(),
            [p_name](gameconf::gcvar_struct const& cgcvar) {
            return cgcvar.name == p_name;
            });
    };

    auto defaultlang_it = get_gcvar_it("defaultlang");

    if(defaultlang_it != gc_vec.cend()) {
        langmod::set_lang(defaultlang_it->value);
    } else missing_gcvar("defaultlang");

    return gc_vec;
}

static void show_lang_prompt(std::array<lang_item, LANG_NUMBER>& p_arr)
{
    int str_line = pcurses::title_y;
    std::string const hint_str("Hint : make a choice by typing the corresponding number.");

    pcurses::display_center_string(hint_str, str_line);
    str_line = display_server::get_last_line() + 3;
    pcurses::display_center_string("Select your language:", str_line);
    str_line = display_server::get_last_line() + 2;

    for(unsigned int i = 1; i <= LANG_NUMBER; ++i) {
        std::string disp_str(std::to_string(i));

        disp_str += ". ";
        disp_str.append(p_arr[i-1].disp);
        pcurses::display_pos_string(disp_str, pcurses::choice_space, str_line);
        str_line = display_server::get_last_line() + 1;
    }

    str_line += 1;
    pcurses::display_pos_string("Your choice: ", 12, str_line);
    display_server::show_screen();
}


//Show a prompt asking the user to choose the language and the prompt to do so
void Game::ask_lang(std::string const& p_langdir, std::filesystem::path const& data_path)
{
    bool validinp = false;

    std::array<lang_item, LANG_NUMBER> langarr {
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
            
            if(intval > 0 && intval <= LANG_NUMBER) {
                std::string lang = langarr[intval - 1].id;

                langmod::set_lang(lang);
                validinp = true;
                pstrings::copy_file_to_vec(p_langdir, data_path);
                if(!game_error::has_encountered_fatal()) m_strings_init = true;
            } else show_err_msg("Nope. (not a valid input)");
        } else if(buf.size() == 0) show_err_msg("Nope. (nothing !)");
        else show_err_msg("Nope. (too long)");
    }
}

Game::~Game()
{
    delwin(stdscr);
    endwin();

    //Print the message attached to a fatal error if one occurred
    if(game_error::has_encountered_fatal()) {
        printf("FATAL ERROR: %s\n", game_error::get_fatal_error_msg().c_str());
    }
}

GameInitData Game::init()
{
    set_curses();
    auto p_paths = files_path::getpaths();
    game_error::set_filepath(p_paths.local_data_path);

    auto gc_vec = fetch_gameconf_vars(p_paths.data_path);

    auto get_gcvar_it = [gc_vec](std::string const& p_name) {
        return std::find_if(gc_vec.cbegin(), gc_vec.cend(),
            [p_name](gameconf::gcvar_struct const& ccpvar) {
            return ccpvar.name == p_name;
            });
    };

    GameInitData game_init_data;

    auto langdir_it = get_gcvar_it("langdir");
    auto roomfile_it = get_gcvar_it("roomfile");
    auto csfile_it = get_gcvar_it("csfile");
    auto firstroom_it = get_gcvar_it("firstroom");

    if(langdir_it != gc_vec.cend()) ask_lang(langdir_it->value, p_paths.data_path);
    else {
        game_init_data.no_error = false;
        missing_gcvar("langdir");
        return game_init_data;
    }
    if(game_error::has_encountered_fatal()) {
        game_init_data.no_error = false;
        return game_init_data;
    }

    if(roomfile_it != gc_vec.cend()) {
        game_init_data.room_file_path = p_paths.data_path;
        game_init_data.room_file_path += roomfile_it->value;
    } else {
        game_init_data.no_error = false;
        missing_gcvar("roomfile");
        return game_init_data;
    }
    if(game_error::has_encountered_fatal()) {
        game_init_data.no_error = false;
        return game_init_data;
    }

    if(csfile_it != gc_vec.cend()) cutscenes::copy_file_to_vec(csfile_it->value,
            p_paths.data_path);
    else {
        game_init_data.no_error = false;
        missing_gcvar("csfile");
        return game_init_data;
    }
    if(game_error::has_encountered_fatal()) {
        game_init_data.no_error = false;
        return game_init_data;
    }

    if(firstroom_it != gc_vec.cend()) {
        game_init_data.no_error = true;
        m_start_room = firstroom_it->value;
        return game_init_data;
    } else {
        game_init_data.no_error = false;
        missing_gcvar("firstroom");
        return game_init_data;
    }
}

void Game::run(GameInitData const& game_init_data)
{
    if(!game_error::has_encountered_fatal()) {
        RoomManager rmm(game_init_data.room_file_path);

        if(!game_error::has_encountered_fatal()) rmm.startLoop(m_start_room);
    }
}