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

#ifndef SAVE_HPP
#define SAVE_HPP

#include <filesystem>

#include "player/Player.hpp"

namespace savefile {
    enum class loading_error {
        NO_FILE,
        NOT_JSON,
        BAD_FORMAT,
        BAD_GAME_NAME,
        NOERROR
    };

    struct load_data {
        std::vector<inventory::gitem> gitems;
        gvarVector gvars;
        std::string current_room;
        loading_error error;
    };
    bool save(Player const& player, std::string const& current_room,
            std::filesystem::path const& local_data_path);
    bool load(savefile::load_data& savefile_data,
            std::filesystem::path const& local_data_path);
}
#endif
