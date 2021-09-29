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

#ifndef GAME_LANG_HPP
#define GAME_LANG_HPP

#include "LocalConfVars.hpp"

namespace game_lang {
    struct LangInfo {
        std::string code;
    };

    void lang_init(LocalConfVars::lcv_data_ptr const& lcv,
            game_lang::LangInfo& lang_info);
}

#endif
