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

#ifndef GAME_HPP
#define GAME_HPP

#include "CutscenesContainer.hpp"
#include "LocalConfVars.hpp"
#include "pargs.hpp"
#include "room/rooms.hpp"

class Game
{
    public:

        ~Game();
        bool init(pargs::args_data const& args_data);
        bool run();

    private:

        std::string m_start_room;
        pstrings::ps_data_ptr pstrings_data;
        rooms::RoomsData_ptr rooms_data;
        CutscenesContainer m_cutscenes_container;

        void ask_lang(LocalConfVars::lcv_data_ptr lcv_data,
                std::string const& data_path);
};
#endif
