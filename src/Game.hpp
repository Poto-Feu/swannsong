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
    along with SwannSong Adventure.
    If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef GAME_HPP
#define GAME_HPP

#include "vars/LocalConfVariable.hpp"
#include "CutscenesContainer.hpp"
#include "pargs.hpp"
#include "pstrings.hpp"

struct GameInitData
{
    std::filesystem::path room_file_path;
    bool no_error;
};

class Game
{
    public:

        ~Game();
        GameInitData init(pargsMap pargs_map);
        void run(GameInitData const& game_init_data);

    private:

        std::string m_start_room;

        LocalConfVariableContainer m_lcvc;
        bool m_strings_init = false;
        PStrings m_program_strings;
        CutscenesContainer m_cutscenes_container;

        void ask_lang(std::string const& p_langdir, std::filesystem::path const& data_path);
        void missing_gcvar(std::string const& p_name);
        auto fetch_gameconf_vars(std::filesystem::path const& system_data_path);
};
#endif
