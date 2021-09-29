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

#include <algorithm>

#include "rendering.hpp"
#include "CutsceneClass.hpp"
#include "dialogbox.hpp"
#include "game_error.hpp"
#include "game_menu.hpp"
#include "pcurses.hpp"
#include "room/RoomClass.hpp"
#include "room/RoomDisplay.hpp"

static void add_string(std::vector<std::string>& strings_vec,
        std::string const& p_str)
{
    std::vector<std::string> string_lines;

    string_lines = pcurses::divide_string_into_lines(p_str);
    strings_vec.insert(strings_vec.end(),
            string_lines.begin(), string_lines.end());
}

void rendering::display_cutscene(pstrings::ps_data_ptr const& pstrings_data,
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
                dialogbox::display(NULL, &strings_vec, pstrings_data);
                strings_vec.clear();
                break;
        }
    }

    dialogbox::display(NULL, &strings_vec, pstrings_data);
    display_server::clear_screen();
}

void rendering::display_inventory(pstrings::ps_data_ptr const& pstrings_data,
        inventory::Inventory const& inv)
{
    std::vector<std::string> strings_list;

    if(inv.size() == 0) {
        strings_list.push_back(pstrings::fetch_string(pstrings_data,
                    "inventory_empty"));
    } else {
        for(auto const& it : inv) {
            std::string str_name = "item_" + it.name;
            std::string disp_str = pstrings::fetch_string(pstrings_data,
                    str_name);

            disp_str += "   " + std::to_string(return_item_n(inv, it.name));
            strings_list.push_back(std::move(disp_str));
        }
    }
    dialogbox::display(nullptr, &strings_list, pstrings_data);
}

Choice const* get_choice_from_vector(std::vector<Choice> const& choices,
        unsigned int choice_id)
{
    auto const& choice_it = std::find_if(choices.cbegin(), choices.cend(),
            [=](Choice const& choice) {
            return choice_id == choice.getId();
            });

    if(choice_it == choices.cend()) {
        return nullptr;
    } else {
        return &*choice_it;
    }
}

std::string rendering::display_room(pstrings::ps_data_ptr const& pstrings_data,
        Room const& room, RoomDisplay const& room_display,
        const std::string *error_msg)
{
    std::string const* room_title_str = nullptr;
    std::string const* room_desc_str = nullptr;
    std::vector<Choice> const& room_choices = room.getChoicesVec();
    std::vector<std::string> room_choices_str;
    game_menu::flags menu_flags;

    if(room_display.show_title) {
        room_title_str = room.getTitle();
    }

    if(room_display.show_desc) {
        room_desc_str = room.getDesc();
    }

    if(room_display.are_all_choices_displayed) {
        std::transform(room_choices.begin(), room_choices.end(),
                std::back_inserter(room_choices_str),
                [](Choice const& p_choice) {
                return p_choice.getText();
                });
    } else if(room_display.choices_displayed.size() != 0) {
        for(auto const& it : room_display.choices_displayed) {
            Choice const* current_choice = get_choice_from_vector(room_choices,
                    it);

            if(!current_choice) {
                game_error::emit_warning(std::to_string(it)
                        + " Choice doesn't exist in " + room.getName()
                        + " ROOM");
                continue;
            }

            room_choices_str.push_back(current_choice->getText());
        }
    }

    return game_menu::display(room_title_str, room_desc_str, nullptr,
            room_choices_str, error_msg, &menu_flags, &pstrings_data);
}
