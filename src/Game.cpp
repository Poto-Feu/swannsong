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

#include "Game.hpp"
#include "files_path.hpp"
#include "game_error.hpp"
#include "game_state.hpp"
#include "game_lang.hpp"
#include "gameconf.hpp"
#include "pcurses.hpp"
#include "player/Player.hpp"
#include "room/RoomClass.hpp"
#include "room/RoomLoopState.hpp"

static void missing_gcvar(std::string const& p_name)
{
    game_error::fatal_error("missing gameconf var (" + p_name +")");
}

Game::~Game()
{
    pcurses::clean();

    //Print the message attached to a fatal error if one occurred
    if(game_error::has_encountered_fatal()) {
        printf("FATAL ERROR: %s\n", game_error::get_fatal_error_msg().c_str());
    }
}

bool Game::init(pargs::args_data const& args_data)
{
    LocalConfVars::lcv_data_ptr lcv;
    game_lang::LangInfo lang_info;
    std::vector<std::string> const* csfile_values;
    std::vector<std::string> const* firstroom_values;

    files_path::paths_struct p_paths = files_path::getpaths(args_data.local);

    if(!args_data.reset) {
        lcv = LocalConfVars::init_data(p_paths.local_conf_path);
    } else {
        lcv = LocalConfVars::init_data(p_paths.local_conf_path, true);
    }

    if(!lcv) {
        return false;
    }

    game_error::set_filepath(p_paths.local_data_path);

    gameconf::gcvars_ptr gameconf_vars = gameconf::readfile(p_paths.data_path);

    if(game_error::has_encountered_fatal()) {
        return false;
    }

    pcurses::init();
    game_lang::lang_init(lcv, lang_info);

    csfile_values = gameconf::get_var(gameconf_vars, "csfile");
    firstroom_values = gameconf::get_var(gameconf_vars, "firstroom");
    if(!csfile_values) {
        missing_gcvar("csfile");
        return false;
    } else if(!firstroom_values) {
        missing_gcvar("firstroom");
        return false;
    }

    if(!init_game_data(p_paths.data_path, lang_info.code, this->game_data)
            || game_error::has_encountered_fatal()) {
        return false;
    } else if(!LocalConfVars::write_to_file(lcv, p_paths.local_conf_path)) {
        return false;
    } else {
        m_start_room = (*firstroom_values)[0];
        return true;
    }
}

bool Game::run()
{
    RoomLoopState rls;
    Player player;
    game_state_s game_state;
    std::string current_room_id = this->m_start_room;

    while(!game_state.should_game_exit) {
        Room const* room = rooms::get_room(this->game_data.rooms_data,
                current_room_id);

        if(!room) {
            game_error::fatal_error(current_room_id
                    + " ROOM not present in rooms file");
            display_server::clear_screen();
            return false;
        }

        rls.resetGameOver();

        if(!room->load(game_data, player, rls, game_state)) {
            return false;
        }
        if(!game_state.should_game_exit) {
            current_room_id = game_state.next_room;
        }
    }
    display_server::clear_screen();

    return true;
}
