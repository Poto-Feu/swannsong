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

#include <cstdint>
#include <stdexcept>
#include "lang.hpp"

namespace langmod
{
    static const uint8_t LANG_SIZE = 3;

    static std::string game_lang;
    static std::string langdir;

    std::string get_lang()
    {
        return game_lang;
    }

    void set_lang(std::string const& p_lang)
    {
        if(p_lang.size() != LANG_SIZE) throw std::runtime_error("incorrect lang string size (" +
                p_lang + ")");
        else game_lang = p_lang;
    }
}
