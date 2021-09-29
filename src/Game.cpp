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
#include "game_state.hpp"
#include "game_lang.hpp"
#include "pcurses.hpp"
#include "player/Player.hpp"
#include "rendering.hpp"
#include "room/RoomClass.hpp"
#include "room/RoomLoopState.hpp"

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
    game_lang::LangInfo lang_info;

    files_path::paths_struct p_paths = files_path::getpaths(args_data.local);

    if(!args_data.reset) {
        lcv = LocalConfVars::init_data(p_paths.local_conf_path);
    } else {
        lcv = LocalConfVars::init_data(p_paths.local_conf_path, true);
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
    game_lang::lang_init(lcv, lang_info);
    this->pstrings_data = pstrings::init_data(p_paths.data_path,
            lang_info.code);

    auto csfile_it = get_gcvar_it("csfile");
    auto firstroom_it = get_gcvar_it("firstroom");

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
