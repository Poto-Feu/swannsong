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

#include "rendering.hpp"
#include "dialogbox.hpp"
#include "pcurses.hpp"

static void add_string(std::vector<std::string>& strings_vec,
        std::string const& p_str)
{
    std::vector<std::string> string_lines;

    string_lines = pcurses::divide_string_into_lines(p_str);
    strings_vec.insert(strings_vec.end(),
            string_lines.begin(), string_lines.end());
}

void rendering::display_cutscene(PStrings const& program_strings,
        Cutscene const& cutscene)
{
    std::vector<std::string> strings_vec;

    for(auto const& action_it : cutscene.actions_vec) {
        switch(action_it.type) {
            case cs_action_type::STRING:
                add_string(strings_vec, action_it.content);
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

void rendering::display_inventory(inventory::Inventory const& inv,
        PStrings const& pstrings)
{
    std::vector<std::string> strings_list;

    if(inv.size() == 0) {
        strings_list.push_back(pstrings.fetch("inventory_empty"));
    } else {
        for(auto const& it : inv) {
            std::string disp_str = it.name;
            std::string str_name = "item_" + it.name;

            if(pstrings.check_exist(str_name)) {
                disp_str = pstrings.fetch(str_name);
            }

            disp_str += "   " + std::to_string(return_item_n(inv, it.name));
            strings_list.push_back(std::move(disp_str));
        }
    }
    dialogbox::display(nullptr, &strings_list, pstrings);
}
