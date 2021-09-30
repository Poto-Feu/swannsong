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

#include "game_lang.hpp"
#include "game_menu.hpp"
#include "terminal.hpp"

struct lang_item {
    std::string id;
    std::string displayed_name;
};

const std::vector<lang_item> langs {
    { "eng", "English" },
    { "fra", "Français" }
};

/* Display a screen where the user can select its language. The appropriate
 * lang code is returned. */
static std::string const& show_lang_select_screen()
{
    bool valid_input = false;
    int lang_index = -1;
    const std::string *error_msg_ptr = nullptr;

    // Hacky way to get "Français" to display correctly
    terminal::set_locale("fra");

    while(!valid_input) {
        const std::string hint_str
            = "Make a choice by typing the corresponding number.";
        const std::string select_lang_str = "Select your language";
        const std::string not_valid_error_str = "Nope. (not a valid input)";

        game_menu::flags menu_flags;
        menu_flags.title_bold = false;
        menu_flags.input_length = 2;

        std::string buf = game_menu::display(&hint_str, &select_lang_str,
                nullptr, { langs[0].displayed_name, langs[1].displayed_name },
                error_msg_ptr, &menu_flags, nullptr);

        if(buf.size() == 1) {
            unsigned int input_integer = buf[0] - '0';

            if(input_integer > 0 && input_integer <= langs.size()) {
                std::string lang = langs[input_integer - 1].id;

                valid_input = true;
                lang_index = (int)input_integer - 1;
            } else {
                error_msg_ptr = &not_valid_error_str;
            }
        } else {
            error_msg_ptr = &not_valid_error_str;
        }
    }

    return langs[lang_index].id;
}

void game_lang::lang_init(LocalConfVars::lcv_data_ptr const& lcv,
        game_lang::LangInfo& lang_info)
{
    std::string const* lang_var = LocalConfVars::get_value(lcv, "lang");

    if(!lang_var) {
        lang_info.code = show_lang_select_screen();
    } else {
        lang_info.code = *lang_var;
    }

    LocalConfVars::set_value(lcv, "lang", lang_info.code);
    LocalConfVars::set_value(lcv, "firstlaunch", "0");
    terminal::set_locale(lang_info.code);
}
