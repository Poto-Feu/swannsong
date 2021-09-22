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

#include "game_menu.hpp"
#include "pcurses.hpp"
#include "userio.hpp"

std::string game_menu::display(const std::string *title,
        const std::string *desc, const std::vector<std::string> *other_str,
        std::vector<std::string> choices, const std::string *error_msg,
        flags *p_flags, pstrings::ps_data_ptr const* pstrings_data)
{
    flags default_flags;
    if(!p_flags) p_flags = &default_flags;
    if(p_flags->input_length < 1 || p_flags->input_length > default_input_length) {
        p_flags->input_length = default_input_length;
    }

    const int error_msg_lines = pcurses::lines - pcurses::bottom_margin;
    const int error_msg_cols = pcurses::margin;
    const int choice_offset = 8;
    const int prompt_cols_offset = 12;

    int desc_line = pcurses::title_y + 2;
    int other_str_line = desc_line + 2;
    int choices_line = other_str_line;
    int prompt_line = other_str_line + 2;

    display_server::clear_screen();

    if(error_msg) {
        display_server::add_string(*error_msg, { error_msg_lines, error_msg_cols }, A_BOLD);
    }

    if(title) {
        if(p_flags->title_bold) pcurses::display_center_string(*title, pcurses::title_y, A_BOLD);
        else pcurses::display_center_string(*title, pcurses::title_y);

        desc_line = display_server::get_last_line() + 2;
        other_str_line = display_server::get_last_line() + 4;
        choices_line = other_str_line;
        prompt_line = other_str_line + 2;
    }

    if(desc) {
        pcurses::display_center_string(*desc, desc_line);
        other_str_line = display_server::get_last_line() + 2;
        choices_line = other_str_line;
        prompt_line = other_str_line + 2;
    }

    if(other_str) {
        for(auto const& it : *other_str) {
            pcurses::display_center_string(it, other_str_line);
            other_str_line += display_server::get_last_line() + 2;
        }
        if(other_str->size() > 0) {
            choices_line = display_server::get_last_line() + 2;
            prompt_line = choices_line + 2;
        }
    }

    for(size_t i = 0; i < choices.size(); ++i) {
        choices[i] = std::to_string(i + 1) + ". " + choices[i];
        pcurses::display_pos_string(choices[i], choice_offset, choices_line);
        choices_line = display_server::get_last_line() + 1;
        prompt_line = choices_line + 1;
    }

    if(pstrings_data) {
        std::string const& room_prompt_text = pstrings::fetch_string(
                *pstrings_data, "room_prompt_text");
        pcurses::display_pos_string(room_prompt_text, prompt_cols_offset,
                prompt_line);
    } else {
        pcurses::display_pos_string("Your choice: ", prompt_cols_offset,
                prompt_line);
    }

    display_server::show_screen();

    return userio::gettextinput(p_flags->input_length);
}
