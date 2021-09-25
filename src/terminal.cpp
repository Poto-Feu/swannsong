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

#include <clocale>

#include "terminal.hpp"
#include "os_module.hpp"

void terminal::set_locale(std::string const& lang_code)
{
    if(os_module::current_os == os_module::os_type::UNIXLIKE) {
        if(lang_code == "fra") {
            setlocale(LC_ALL, "fr_FR.UTF-8");
        } else {
            setlocale(LC_ALL, "en_US.UTF-8");
        }
    } else {
        if(lang_code == "fra") {
            setlocale(LC_ALL, "fr_FR");
        } else {
            setlocale(LC_ALL, "en_US");
        }
    }
}
