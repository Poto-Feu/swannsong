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
#include <curses.h>
}

#include <algorithm>
#include <cstdlib>

#include "userio.hpp"
#include "dialogbox.hpp"
#include "files_path.hpp"
#include "game_error.hpp"
#include "player/Player.hpp"
#include "rendering.hpp"
#include "room/interpreter/parser.hpp"
#include "savefile.hpp"
#include "stringsm.h"

/*Get user text input and return it in a pointer*/
static void copy_input_to_char_str(char** buf, int max_n)
{
    *buf = (char*)calloc(max_n+1, sizeof(char));

    echo();
    getnstr(*buf, max_n);
    noecho();

    stringsm_chomp(*buf);
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

static const Choice* get_current_choice(std::vector<Choice> const& choices,
        unsigned int corres_choice_id)
{
    auto current_choice = std::find_if(choices.cbegin(), choices.cend(),
            [=](Choice const& p_choice) {
            return p_choice.getId() == corres_choice_id;
            });

    if(current_choice == choices.cend()) {
        return NULL;
    } else {
        return &*current_choice;
    }
}

bool userio::interpret_user_input(PStrings const& pstrings,
        std::unordered_map<std::string, Room> room_map,
        CutscenesContainer const& cs_container, Player& player,
        Room const& room, RoomLoopState& rls, RoomState& room_state,
        game_state_s& game_state, std::string& input,
        unsigned int& incorrect_input_n)
{
    const std::string *error_msg_ptr = nullptr;
    const std::string need_help_str = pstrings.fetch("room_need_help");
    const std::string incorrect_input_str = pstrings.fetch("incorrect_input");

    if(stringsm::is_number(input)) {
        uint32_t choice_digit = std::stoi(input);

        if(choice_digit != 0) {
            const Choice *current_choice;
            auto const& choices_vec = room.getChoicesVec();
            unsigned int corres_choice_id;
            unsigned int start_ln = 0;

            if(room_state.is_all_choices_displayed()) {
                if(choice_digit > choices_vec.size()) {
                    goto bad_input;
                }
                corres_choice_id = choice_digit;
            } else {
                corres_choice_id = room_state.getCorrespondantChoiceId(
                        choice_digit);
                if(corres_choice_id == 0) {
                    goto bad_input;
                }
            }
            current_choice = get_current_choice(room.getChoicesVec(),
                    corres_choice_id);

            if(!current_choice) {
                goto bad_input;
            }

            //Process the input if it is a number corresponding to a choice
            room_state.setBlockType(RoomState::bt::CHOICE);
            parser::exec_until_end(pstrings, room_map, room, player, rls,
                    room_state, game_state, current_choice->getInstructions(),
                    start_ln);

            if(!game_error::has_encountered_fatal()) {
                room_state.displayCutscenes(pstrings, cs_container);
            }
        }
    }

    if(input == "exit") {
        game_state.should_game_exit = true;
    }

    if(input == "load") {
        using namespace savefile;

        savefile::load_data savefile_data;

        if(!load(savefile_data, files_path::get_local_data_path())) {
            std::vector<std::string> dialogbox_strs;

            if(savefile_data.error == loading_error::NO_FILE) {
                dialogbox_strs.push_back(pstrings.fetch("load_nofile"));
            } else {
                dialogbox_strs.push_back(pstrings.fetch("load_corrupted"));
            }

            dialogbox::display(NULL, &dialogbox_strs, pstrings);
        } else {
            game_state.next_room = savefile_data.current_room;
            player.inv = std::move(savefile_data.gitems);
            gvars::replace_vector(player.gvars, savefile_data.gvars);
        }
    } else if(input == "save") {
        if(savefile::save(player, room.getName(),
                files_path::get_local_data_path())) {
            std::vector<std::string> dialogbox_strs = {
                pstrings.fetch("save_success")
            };

            dialogbox::display(NULL, &dialogbox_strs, pstrings);
        }
    } else if(input == "help") {
        auto const* cs = cs_container.get_cutscene("help");

        rendering::display_cutscene(pstrings, *cs);
    } else if(input == "inv" || input == "inventory") {
        rendering::display_inventory(player.inv, pstrings);
    }

    return true;
bad_input:
    if(incorrect_input_n < 3) {
        error_msg_ptr = &incorrect_input_str;
        ++incorrect_input_n;
    } else error_msg_ptr = &need_help_str;

    input = room_state.displayRoomScreen(pstrings, &room.getName(),
            &room.getTitle(), &room.getDesc(), room.getChoicesVec(),
            error_msg_ptr);
    return false;
}
