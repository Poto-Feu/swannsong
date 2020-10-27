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

#include "CutsceneClass.hpp"
#include "dialogbox.hpp"
#include "display_server.hpp"
#include "pcurses.hpp"
#include "userio.h"

Cutscene::Cutscene() { }

static void add_string(std::string const& p_str, std::vector<std::string>& strings_vec)
{
    std::vector<std::string> string_lines = pcurses::divide_string_into_lines(p_str);

    strings_vec.insert(strings_vec.end(), string_lines.begin(), string_lines.end());
}

void Cutscene::display(PStrings const& program_strings) const
{
    std::vector<std::string> strings_vec;

    for(auto const& action_it : actions_vec) {
        switch(action_it.type) {
            case cs_action_type::STRING:
                add_string(action_it.content, strings_vec);
                break;
            case cs_action_type::BLANK:
                strings_vec.push_back("");
                break;
            case cs_action_type::PAUSE:
                dialogbox::display(NULL, &strings_vec, program_strings);
                strings_vec.clear();
                break;
        }
    }

    dialogbox::display(NULL, &strings_vec, program_strings);
    display_server::clear_screen();
}
