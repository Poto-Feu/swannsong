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
    along with SwannSong Adventure. If not, see
    <https://www.gnu.org/licenses/>.
*/

#include <cstdio>

#include "Game.hpp"
#include "game_error.hpp"

int main (int argc, char **argv)
{
    Game game;
    pargs::args_data args_data;

    pargs::init_data(args_data, argc, argv);

    if(args_data.debug) {
        game_error::setdebug();
    }

    if(game.init(args_data)) {
        if(!game.run()) {
            puts(game_error::get_fatal_error_msg().c_str());
            return EXIT_FAILURE;
        }
    } else {
        puts(game_error::get_fatal_error_msg().c_str());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
