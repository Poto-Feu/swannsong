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

extern "C" {
#include <ncurses.h>
}

#include <cstdlib>

#include "userio.hpp"
#include "CutscenesContainer.hpp"
#include "dialogbox.hpp"
#include "files_path.hpp"
#include "game_error.hpp"
#include "game_state.hpp"
#include "player/Player.hpp"
#include "rendering.hpp"
#include "room/RoomClass.hpp"
#include "room/RoomDisplay.hpp"
#include "room/interpreter/parser.hpp"
#include "savefile.hpp"
#include "stringsm.hpp"

/*Get user text input and return it in a pointer*/
static void copy_input_to_char_str(char** buf, int max_n)
{
    *buf = (char*)calloc(max_n+1, sizeof(char));

    echo();
    getnstr(*buf, max_n);
    noecho();

    stringsm::chomp(*buf);
}

/*Pause the program until the user press Enter*/
void userio::waitenter()
{
    #ifdef _WIN32
    int enter_ch = WIN_ENTER_KEY;
    #else
    int enter_ch = '\n';
    #endif

    while(getch() != enter_ch) {}
}

std::string userio::gettextinput(int max_n)
{
    char* buf = NULL;
    std::string r_str;

    copy_input_to_char_str(&buf, max_n);
    r_str.assign(buf);

    free(buf);

    return r_str;
}

bool userio::interpret_user_input(pstrings::ps_data_ptr const& pstrings_data,
        rooms::RoomsData_ptr const& rooms_data,
        CutscenesContainer const& cs_container, Room const& room,
        Player& player, RoomDisplay const& room_display, RoomLoopState& rls,
        game_state_s& game_state, std::string& menu_input, bool& wrong_input)
{
    wrong_input = false;

    if(stringsm::is_number(menu_input)) {
        uint32_t choice_digit = std::stoi(menu_input);

        if(choice_digit != 0) {
            const Choice *current_choice;
            std::vector<std::string> displayed_cs;
            int corres_choice_id;
            unsigned int start_ln = 0;

            if(room_display.are_all_choices_displayed) {
                if(choice_digit > room.getChoicesVec().size()) {
                    wrong_input = true;
                    return true;
                } else {
                    corres_choice_id = choice_digit;
                }
            } else {
                if(choice_digit > room_display.choices_displayed.size()) {
                    wrong_input = true;
                    return true;
                } else {
                    corres_choice_id =
                        room_display.choices_displayed[choice_digit - 1];
                }
            }

            current_choice = room.getChoice(corres_choice_id);

            //Process the input if it is a number corresponding to a choice
            if(!parser::exec_until_end(current_choice->getInstructions(),
                    rooms_data, room, parser::block_type::CHOICE, player, rls,
                    nullptr, game_state, displayed_cs, start_ln)) {
                return false;
            }

            for(auto const& it : displayed_cs) {
                auto const* cs = cs_container.get_cutscene(it);

                if(!cs) {
                    game_error::emit_warning("Unknown cutscene");
                } else {
                    rendering::display_cutscene(pstrings_data, *cs);
                }
            }
        } else {
            wrong_input = true;
        }
    } else if(menu_input == "exit") {
        game_state.should_game_exit = true;
    } else if(menu_input == "load") {
        using namespace savefile;

        savefile::load_data savefile_data;

        if(!load(savefile_data, files_path::get_local_data_path())) {
            std::vector<std::string> dialogbox_strs;

            if(savefile_data.error == loading_error::NO_FILE) {
                dialogbox_strs.push_back(pstrings::fetch_string(pstrings_data,
                            "load_nofile"));
            } else {
                dialogbox_strs.push_back(
                        pstrings::fetch_string(pstrings_data,
                            "load_corrupted"));
            }

            dialogbox::display(NULL, &dialogbox_strs, pstrings_data);
        } else {
            game_state.next_room = savefile_data.current_room;
            player.inv = std::move(savefile_data.gitems);
            gvars::replace_vector(player.gvars, savefile_data.gvars);
        }
    } else if(menu_input == "save") {
        if(savefile::save(player, room.getName(),
                files_path::get_local_data_path())) {
            std::vector<std::string> dialogbox_strs = {
                pstrings::fetch_string(pstrings_data, "save_success")
            };

            dialogbox::display(NULL, &dialogbox_strs, pstrings_data);
        }
    } else if(menu_input == "help") {
        auto const* cs = cs_container.get_cutscene("help");

        rendering::display_cutscene(pstrings_data, *cs);
    } else if(menu_input == "inv" || menu_input == "inventory") {
        rendering::display_inventory(pstrings_data, player.inv);
    } else {
        wrong_input = true;
    }

    return true;
}
