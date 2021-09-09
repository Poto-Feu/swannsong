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

#include "room/RoomClass.hpp"
#include "files_path.hpp"
#include "savefile.hpp"
#include "room/interpreter/parser.hpp"
#include "room/room_struct.hpp"
#include "CutscenesContainer.hpp"
#include "dialogbox.hpp"
#include "game_error.hpp"
#include "pstrings.hpp"
#include "stringsm.h"

Room::Room() { }
Room::Room(std::string const& room_name) : m_name(room_name) { }
Room::Room(std::string const& room_name, std::string const& room_title) : m_name(room_name),
    m_title(room_title) { }
Room::Room(std::string const& room_name, std::string const& room_title,
    std::string const& room_desc) : m_name(room_name), m_title(room_title),
    m_desc(room_desc) { }

std::string const& Room::getName() const
{
    return m_name;
}

std::string const& Room::getTitle() const
{
    return m_title;
}

std::string const& Room::getDesc() const
{
    return m_desc;
}

std::vector<TokenVec> const& Room::getATLAUNCHIns() const
{
    return m_ATLAUNCH_ins;
}

std::vector<Choice> const& Room::getChoicesVec() const
{
    return m_Choices_vec;
}

const Choice *Room::getChoice(unsigned int choice_n) const
{
    auto const& Choice_it = std::find_if(m_Choices_vec.cbegin(), m_Choices_vec.cend(),
            [&](Choice const& p_choice) {
            return choice_n == p_choice.getId();
            });

    if(Choice_it == m_Choices_vec.cend()) return nullptr;
    else return &(*Choice_it);
}

void Room::setDesc(std::string const& room_desc)
{
    m_desc = room_desc;
}

void Room::setATLAUNCH_ins(std::vector<TokenVec>&& atlaunch_ins)
{
    m_ATLAUNCH_ins = std::move(atlaunch_ins);
}

void Room::setChoices(std::vector<Choice>&& choices_vec)
{
    m_Choices_vec = std::move(choices_vec);
}

bool Room::isChoicePresent(unsigned int choice_n) const
{
    return std::find_if(m_Choices_vec.cbegin(), m_Choices_vec.cend(), [=](Choice const& p_Choice) {
            return p_Choice.getId() == choice_n;
            }) != m_Choices_vec.cend();
}

static void display(room_struct& p_struct, bool same_room)
{
    const std::string *error_msg_ptr = nullptr;
    const std::string incorrect_input_str = p_struct.program_strings.fetch("incorrect_input");
    const std::string need_help_str = p_struct.program_strings.fetch("room_need_help");

    unsigned int incorrect_input = 0;

    std::string menu_input = p_struct.currState.displayAll(p_struct.currRoom,
            p_struct.program_strings, p_struct.cutscenes_container, same_room);

    while(true) {
        if(stringsm::is_number(menu_input)) {
            uint32_t choice_digit = std::stoi(menu_input);

            if(choice_digit != 0) {
                auto const& choices_vec = p_struct.currRoom.getChoicesVec();
                unsigned int corres_Choice_id;

                if(p_struct.currState.is_all_choices_displayed()) {
                    if(choice_digit > choices_vec.size()) goto bad_input;
                    corres_Choice_id = choice_digit;
                } else {
                    corres_Choice_id = p_struct.currState.getCorrespondantChoiceId(
                            choice_digit);
                    if(corres_Choice_id == 0) goto bad_input;
                }
                auto current_choice = std::find_if(choices_vec.cbegin(), choices_vec.cend(),
                        [=](Choice const& p_choice) {
                        return p_choice.getId() == corres_Choice_id;
                        });

                if(current_choice == choices_vec.cend()) goto bad_input;

                //Process the input if it is a number corresponding to a choice
                unsigned int start_ln = 0;

                p_struct.currState.setBlockType(RoomState::bt::CHOICE);
                parser::exec_until_end(current_choice->getInstructions(),
                        p_struct, start_ln);

                if(!game_error::has_encountered_fatal()) {
                    p_struct.currState.displayCutscenes(
                            p_struct.program_strings,
                            p_struct.cutscenes_container);
                }
            }
        } else if(menu_input == "exit") {
            p_struct.currLoopState.endLoop();
        } else if(menu_input == "load") {
            using namespace savefile;

            savefile::load_data savefile_data;

            if(!load(savefile_data, files_path::get_local_data_path())) {
                std::vector<std::string> dialogbox_strs;

                if(savefile_data.error == loading_error::NO_FILE) {
                    dialogbox_strs.push_back(p_struct.program_strings.fetch(
                                "load_nofile"));
                } else {
                    dialogbox_strs.push_back(p_struct.program_strings.fetch(
                                "load_corrupted"));
                }

                dialogbox::display(NULL, &dialogbox_strs,
                        p_struct.program_strings);
            } else {
                p_struct.currLoopState.setNextRoom(savefile_data.current_room);
                p_struct.currPlayer.inv = std::move(savefile_data.gitems);
                gvars::replace_vector(p_struct.currPlayer.gvars,
                        savefile_data.gvars);
            }
        } else if(menu_input == "save") {
            if(savefile::save(p_struct.currPlayer, p_struct.currRoom.getName(),
                    files_path::get_local_data_path())) {
                std::vector<std::string> dialogbox_strs = {
                    p_struct.program_strings.fetch("save_success")
                };

                dialogbox::display(NULL, &dialogbox_strs,
                        p_struct.program_strings);
            }
        } else if(menu_input == "help") {
            p_struct.cutscenes_container.display(p_struct.program_strings,
                    "help");
        } else if(menu_input == "inv" || menu_input == "inventory") {
            inventory::display_screen(p_struct.currPlayer.inv, p_struct.program_strings);
        }

        return;
bad_input:
        if(incorrect_input < 3) {
            error_msg_ptr = &incorrect_input_str;
            ++incorrect_input;
        } else error_msg_ptr = &need_help_str;

        menu_input = p_struct.currState.displayRoomScreen(p_struct.currRoom,
                p_struct.program_strings, error_msg_ptr);
    }
}

//Read the first ATLAUNCH block encountered starting from specified line
static void atlaunch(room_struct& p_struct, bool same_room)
{
    unsigned int foundln = 0;

    p_struct.currState.setBlockType(RoomState::bt::ATLAUNCH);
    parser::exec_until_end(p_struct.currRoom.getATLAUNCHIns(), p_struct, foundln);

    if(game_error::has_encountered_fatal()) return;
    else if(!p_struct.currLoopState.is_game_over()) display(p_struct, same_room);
    else {
        p_struct.currState.displayCutscenes(p_struct.program_strings, p_struct.cutscenes_container);
    }
}

bool Room::load(RoomLoopState& p_rls, Player& p_player,
        std::unordered_map<std::string, Room> const& room_map, PStrings const& program_strings,
        CutscenesContainer const& cutscenes_container) const
{
    bool same_room = false;
    do {
        RoomState currentState;
        room_struct p_struct { *this, currentState, p_rls, p_player, room_map, program_strings,
        cutscenes_container };

        p_rls.setNextRoom(m_name);

        atlaunch(p_struct, same_room);
        if(game_error::has_encountered_fatal()) return false;
        else if(p_struct.currLoopState.is_game_over()) gameOver(p_player, p_struct.currLoopState,
                program_strings);
        else same_room = true;
    } while(p_rls.getNextRoom() == m_name && !p_rls.is_endgame() && !p_rls.is_unfinished());

    return true;
}

void Room::gameOver(Player& player, RoomLoopState& loop_state, PStrings const& program_strings)
    const
{
    const std::string gameover_title = "GAME OVER";

    const std::vector<std::string> gameover_strings {
        program_strings.fetch("gameover_msg")
    };
    inventory::clear(player.inv);
    gvars::clear(player.gvars);
    /*Kinda bad implementation since it breaks the separation between code and data, but
    that'll do for now - the whole structure of the code is janky anyway*/
    loop_state.setNextRoom("menu");
    dialogbox::display(&gameover_title, &gameover_strings, program_strings);
}
