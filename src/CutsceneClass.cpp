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
#include "display_server.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "userio.h"

Cutscene::Cutscene() { }

void Cutscene::display() const
{
    execute_all_actions();
}

void Cutscene::execute_all_actions() const
{
    int str_line = pcurses::top_margin;

    for(auto const& action_it : actions_vec) {
        switch(action_it.type) {
            case cs_action_type::STRING:
                pcurses::display_center_string(action_it.content, str_line);
                str_line = display_server::get_last_line() + 1;
                break;
            case cs_action_type::BLANK:
                ++str_line;
                break;
            case cs_action_type::PAUSE:
                pcurses::display_penter_message();
                str_line = pcurses::top_margin;
                break;
        }
    }

    pcurses::display_penter_message();
    display_server::clear_screen();
}
