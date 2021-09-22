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

#include "Game.hpp"
#include "game_error.hpp"

int main (int argc, char **argv)
{
    Game game;
    GameInitData game_init_data;
    pargs::args_data args_data;

    pargs::init_data(args_data, argc, argv);

    if(args_data.debug) {
        game_error::setdebug();
    }

    game_init_data = game.init(args_data);

    if(game_init_data.no_error) {
        game.run(game_init_data);
    }

    return 0;
}
