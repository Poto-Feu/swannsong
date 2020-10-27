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
#include "game_menu.hpp"
#include "pcurses.hpp"
#include "userio.h"

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
    keypad(stdscr, TRUE);

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
        m_lcvc.changeValue("lang", defaultlang_it->value);
    } else missing_gcvar("defaultlang");

    return gc_vec;
}

//Show a prompt asking the user to choose the language and the prompt to do so
void Game::ask_lang(std::string const& p_langdir, std::filesystem::path const& data_path)
{
    bool validinp = false;

    std::vector<lang_item> langarr {
        lang_item("eng", "English"),
        lang_item("fra", "Français")
    };

    const std::string *error_msg_ptr = nullptr;

    while(!validinp) {
        const std::string hint_str = "Hint : make a choice by typing the corresponding number.";
        const std::string select_lang_str = "Select your language";
        const std::string not_valid_error_str = "Nope. (not a valid input)";
        const std::string nothing_error_str = "Nope. (nothing !)";
        const std::string too_long_error_str = "Nope. (too long)";

        game_menu::flags menu_flags;
        menu_flags.title_bold = false;
        menu_flags.input_length = 2;

        std::string buf = game_menu::display(&hint_str, &select_lang_str, nullptr,
                { langarr[0].disp, langarr[1].disp }, error_msg_ptr, &menu_flags, nullptr);

        if(buf.size() == 1) {
            size_t intval = buf[0] - '0';
            
            if(intval > 0 && intval <= langarr.size()) {
                std::string lang = langarr[intval - 1].id;

                validinp = true;
                m_lcvc.changeValue("lang", langarr[intval - 1].id);
                m_program_strings = PStrings(langarr[intval - 1].id, p_langdir, data_path);
                if(!game_error::has_encountered_fatal()) m_strings_init = true;
            } else error_msg_ptr = &not_valid_error_str;
        } else if(buf.size() == 0) error_msg_ptr = &nothing_error_str;
        else error_msg_ptr = &too_long_error_str;
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

GameInitData Game::init(pargsMap pargs_map)
{
    files_path::paths_struct p_paths = files_path::getpaths(pargs_map["local"]);
    m_lcvc = LocalConfVariableContainer(p_paths.local_conf_path.string());

    if(pargs_map["reset"]) {
        m_lcvc.deleteFile();
        m_lcvc.reset();
    }
    game_error::set_filepath(p_paths.local_data_path);

    set_curses();

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

    if(langdir_it != gc_vec.cend()) {
        if(m_lcvc.getValue("firstlaunch") == "1") ask_lang(langdir_it->value, p_paths.data_path);
        else {
            m_program_strings = PStrings(m_lcvc.getValue("lang"), langdir_it->value,
                    p_paths.data_path);
        }
    } else {
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

    if(csfile_it != gc_vec.cend()) cutscenes::copy_file_to_vec(csfile_it->value, p_paths.data_path,
            m_program_strings);
    else {
        game_init_data.no_error = false;
        missing_gcvar("csfile");
        return game_init_data;
    }
    if(game_error::has_encountered_fatal()) {
        game_init_data.no_error = false;
        return game_init_data;
    }

    if(m_lcvc.getValue("firstlaunch") == "1") {
        cutscenes::display("help", m_program_strings);
        m_lcvc.changeValue("firstlaunch", "0");
        m_lcvc.writeToFile();
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
        RoomManager rmm(game_init_data.room_file_path, std::move(m_program_strings));

        if(!game_error::has_encountered_fatal()) rmm.startLoop(m_start_room);
    }
}
