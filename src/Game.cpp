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

#include <algorithm>

#include "Game.hpp"
#include "fileio/gameconf.hpp"
#include "files_path.hpp"
#include "game_error.hpp"
#include "game_menu.hpp"
#include "game_state.hpp"
#include "os_module.hpp"
#include "pcurses.hpp"
#include "player/Player.hpp"
#include "rendering.hpp"
#include "room/RoomClass.hpp"
#include "room/RoomLoopState.hpp"
#include "terminal.hpp"
#include "userio.hpp"

struct lang_item
{
    lang_item(std::string const& p_id, std::string const& p_disp)
        : id(p_id), disp(p_disp) {}

    std::string id;
    std::string disp;
};

static void missing_gcvar(std::string const& p_name)
{
    game_error::fatal_error("missing gameconf var (" + p_name +")");
}

static void missing_lcvar(std::string const& p_name)
{
    game_error::fatal_error("missing local conf var (" + p_name +")");
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
static auto fetch_gameconf_vars(LocalConfVars::lcv_data_ptr lcv,
        std::string const& system_data_path)
{
    std::string const* firstlaunch_val;
    auto gc_vec = gameconf::readfile(system_data_path);

    auto get_gcvar_it = [gc_vec](std::string const& p_name) {
        return std::find_if(gc_vec.cbegin(), gc_vec.cend(),
            [p_name](gameconf::gcvar_struct const& cgcvar) {
            return cgcvar.name == p_name;
            });
    };

    //Avoid overwriting user-selected language
    firstlaunch_val = LocalConfVars::get_value(lcv, "firstlaunch");
    if(!firstlaunch_val) {
        missing_lcvar("firstlaunch");
    } else if(*firstlaunch_val == "1") {
        auto defaultlang_it = get_gcvar_it("defaultlang");

        if(defaultlang_it != gc_vec.cend()) {
            LocalConfVars::change_value(lcv, "lang",
                    defaultlang_it->value);
        } else {
            missing_gcvar("defaultlang");
        }
    }

    return gc_vec;
}

//Show a prompt asking the user to choose the language and the prompt to do so
void Game::ask_lang(LocalConfVars::lcv_data_ptr lcv,
        std::string const& data_path)
{
    bool validinp = false;

    std::vector<lang_item> langarr {
        lang_item("eng", "English"),
        lang_item("fra", "Français")
    };

    const std::string *error_msg_ptr = nullptr;

    // Slight hack to get "Français" to display correctly
    terminal::set_locale("fra");

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
                LocalConfVars::change_value(lcv, "lang",
                        langarr[intval - 1].id);
                this->pstrings_data = pstrings::init_data(data_path,
                        langarr[intval - 1].id);
            } else error_msg_ptr = &not_valid_error_str;
        } else if(buf.size() == 0) {
            error_msg_ptr = &nothing_error_str;
        } else {
            error_msg_ptr = &too_long_error_str;
        }
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

bool Game::init(pargs::args_data const& args_data)
{
    LocalConfVars::lcv_data_ptr lcv;

    files_path::paths_struct p_paths = files_path::getpaths(args_data.local);

    if(!args_data.reset) {
        lcv= LocalConfVars::init_data(p_paths.local_conf_path);
    } else {
        lcv= LocalConfVars::init_data(p_paths.local_conf_path, true);
    }
    game_error::set_filepath(p_paths.local_data_path);

    // If this function succeed, "firstlaunch" is guaranteed to exists
    auto gc_vec = fetch_gameconf_vars(lcv, p_paths.data_path);

    if(game_error::has_encountered_fatal()) {
        return false;
    }

    auto get_gcvar_it = [gc_vec](std::string const& p_name) {
        return std::find_if(gc_vec.cbegin(), gc_vec.cend(),
            [p_name](gameconf::gcvar_struct const& ccpvar) {
            return ccpvar.name == p_name;
            });
    };

    set_curses();

    auto csfile_it = get_gcvar_it("csfile");
    auto firstroom_it = get_gcvar_it("firstroom");

    if(*LocalConfVars::get_value(lcv, "firstlaunch") == "1") {
        ask_lang(lcv, p_paths.data_path);
    } else {
        std::string const* lang_lcv = LocalConfVars::get_value(lcv,
                "lang");
        if(!lang_lcv) {
            missing_lcvar("lang");
            return false;
        }

        this->pstrings_data = pstrings::init_data(p_paths.data_path,
                *lang_lcv);
        if(!this->pstrings_data) {
            return false;
        }
    }

    this->rooms_data = rooms::init_data(this->pstrings_data,
            p_paths.data_path);
    if(!this->rooms_data) {
        return false;
    }

    if(csfile_it != gc_vec.cend()) {
        m_cutscenes_container = CutscenesContainer(csfile_it->value,
                p_paths.data_path, this->pstrings_data);
    } else {
        missing_gcvar("csfile");
        return false;
    }

    if(game_error::has_encountered_fatal()) {
        return false;
    }

    if(*LocalConfVars::get_value(lcv, "firstlaunch") == "1") {
        auto const* cs = m_cutscenes_container.get_cutscene("help");

        rendering::display_cutscene(this->pstrings_data, *cs);
        LocalConfVars::change_value(lcv, "firstlaunch", "0");
        LocalConfVars::write_to_file(lcv, p_paths.local_conf_path);
    }

    if(firstroom_it != gc_vec.cend()) {
        m_start_room = firstroom_it->value;
        return true;
    } else {
        missing_gcvar("firstroom");
        return false;
    }
}

bool Game::run()
{
    RoomLoopState rls;
    Player player;
    game_state_s game_state;
    std::string current_room_id = this->m_start_room;

    while(!game_state.should_game_exit) {
        Room const* room = rooms::get_room(this->rooms_data,
                current_room_id);

        if(!room) {
            game_error::fatal_error(current_room_id
                    + " ROOM not present in rooms file");
            display_server::clear_screen();
            return false;
        }

        rls.resetGameOver();

        if(!room->load(pstrings_data, this->rooms_data,
                    this->m_cutscenes_container, player, rls, game_state)) {
            return false;
        }
        if(!game_state.should_game_exit) {
            current_room_id = game_state.next_room;
        }
    }
    display_server::clear_screen();

    return true;
}
