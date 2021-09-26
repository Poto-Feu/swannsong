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

#include "room/interpreter/parser.hpp"
#include "game_error.hpp"
#include "pcurses.hpp"
#include "player/Player.hpp"
#include "room/RoomClass.hpp"
#include "room/RoomDisplay.hpp"
#include "stringsm.hpp"

static void CHOICE_block_err(std::string const& func_name)
{
    game_error::fatal_error(func_name
            + " function cannot be used in CHOICE block");
}

static void wrg_tkn_num(std::string const& func_name)
{
    game_error::fatal_error("wrong number of tokens (" + func_name + ")");
}

//Interpret a line which use the SET function
static bool interp_SET_func(gvarVector& p_gvars, TokenVec const& r_vec)
{
    using namespace game_error;

    if(r_vec.size() != 4) {
        wrg_tkn_num("SET");
        return false;
    } else {
        if(r_vec[2].type != token_type::EQUAL) {
            fatal_error("missing EQUAL token (SET)");
            return false;
        } else if(r_vec[3].type != token_type::NUMBER) {
            fatal_error("no value assigned to var during its init");
            return false;
        } else {
            gvars::set_var(p_gvars, r_vec[1].str, std::stoi(r_vec[3].str));
        }
    }

    return true;
}

//Interpret a line which use the DISPLAY function
static bool interp_DISPLAY_func(TokenVec const& r_vec, Room const& room,
        parser::block_type block_type, RoomDisplay* room_display)
{
    using namespace game_error;

    if(block_type == parser::block_type::CHOICE) {
        CHOICE_block_err("DISPLAY");
        return false;
    } else if(r_vec.size() != 2 && r_vec.size() != 3) {
        wrg_tkn_num("DISPLAY");
        return false;
    } else if(r_vec.size() == 3) {
        if(r_vec[1].str != "CHOICE") {
            fatal_error("incorrect DISPLAY syntax (" + room.getName() + ")");
            return false;
        } else if(r_vec[2].str.length() < 3
                && stringsm::is_number(r_vec[2].str)) {
            int choice_n = stoi(r_vec[2].str);

            if(!room.isChoicePresent(choice_n)) {
                game_error::fatal_error("choice " + r_vec[2].str
                        + " doesn't exist in " + room.getName() + " ROOM");
            } else {
                room_display->choices_displayed.push_back(stoi(r_vec[2].str));
            }
        }
    } else if(r_vec.size() == 2) {
        if(r_vec[1].str == "CHOICES") {
            room_display->are_all_choices_displayed = true;
        } else if(r_vec[1].str == "TITLE") {
            room_display->show_title = true;
        } else if(r_vec[1].str == "DESC") {
            room_display->show_desc = true;
        } else if(r_vec[1].str == "ALL") {
            room_display->show_title = true;
            room_display->show_desc = true;
            room_display->are_all_choices_displayed = true;
        }
    }

    return true;
}

//Interpret a line which use the CUTSCENE function
static bool interp_CUTSCENE_func(TokenVec const& r_vec,
        std::vector<std::string>& cutscenes_vec)
{
    if(r_vec.size() != 2) {
        wrg_tkn_num("CUTSCENE");
        return false;
    }

    cutscenes_vec.push_back(r_vec[1].str);

    return true;
}

//Intepret a line which use the GO function
static bool interp_GO_func(game_state_s& game_state,
        rooms::RoomsData_ptr const& rooms_data, Room const& room,
        TokenVec const& r_vec)
{
    if(r_vec.size() != 2) {
        wrg_tkn_num("GO");
        return false;
    }

    if(!rooms::get_room(rooms_data, r_vec[1].str)) {
        game_error::fatal_error(r_vec[1].str
                + " ROOM does not exist (GO function in " + room.getName()
                + " ROOM)");
        return false;
    }

    game_state.next_room = r_vec[1].str;

    return true;
}

/* Interpret a line which use the GET function, which add an item to the
 * player's inventory */
static bool interp_GET_func(inventory::Inventory& p_inv, TokenVec const& p_vec)
{
    using namespace game_error;

    inventory::item_val_type item_n = 1;
    int item_name_pos = 1;

    if(p_vec.size() == 3) {
        if(p_vec[1].type != token_type::NUMBER) {
            fatal_error("second part of a GET instruction must be a NUMBER or "
                    "an ITEM");
            return false;
        }

        item_name_pos = 2;
        item_n = std::stoi(p_vec[1].str);
    } else if(p_vec.size() != 2) {
        wrg_tkn_num("GET");
        return false;
    }

    inventory::getitem(p_inv, p_vec[item_name_pos].str, item_n);

    return true;
}

static bool interp_USE_func(inventory::Inventory& p_inv, TokenVec const& p_vec)
{
    using namespace game_error;
    inventory::item_val_type item_n = 1;
    int item_name_pos = 1;

    if(p_vec.size() == 3) {
        if(p_vec[1].type != token_type::NUMBER) {
            fatal_error("second part of an USE instruction must be a NUMBER "
                    "or an ITEM");
            return false;
        }

        item_name_pos = 2;
        item_n = std::stoi(p_vec[1].str);
    } else if(p_vec.size() != 2) {
        wrg_tkn_num("USE");
        return false;
    }

    inventory::useitem(p_inv, p_vec[item_name_pos].str, item_n);

    return true;
}

static void interp_GAMEOVER_func(RoomLoopState& p_rls)
{
    p_rls.setGameOver();
}

static void interp_EXIT_func(game_state_s& game_state)
{
    game_state.should_game_exit = true;
}

//Interpret a line which changes a gvar value
static bool interp_gvar_ins(gvarVector& p_gvars, TokenVec r_vec)
{
    using namespace game_error;

    enum class func_tkn_type {
        EQUAL,
        OPERATOR,
        NUMBER
    };

    enum class oper_type {
        NONE,
        PLUS,
        MINUS
    };

    bool continue_func = true;
    int16_t result_value = 0;
    func_tkn_type last_tkn = func_tkn_type::EQUAL;
    oper_type last_oper = oper_type::NONE;

    if(r_vec[1].type != token_type::EQUAL) {
        fatal_error("second token of a gvars instruction must be an equal "
                "sign");
        return false ;
    } else if(r_vec.size() == 3 && r_vec[2].type == token_type::NUMBER) {
        gvars::change_val(p_gvars, r_vec[0].str, std::stoi(r_vec[2].str));
    }

    for(size_t i = 2; i < r_vec.size() && continue_func; ++i) {
        auto number_tkn_lambda = [=, &r_vec, &result_value, &last_tkn]() {
            switch(last_tkn) {
                case func_tkn_type::EQUAL:
                    result_value = std::stoi(r_vec[i].str);
                    break;
                case func_tkn_type::OPERATOR:
                    switch(last_oper) {
                        case oper_type::PLUS:
                            result_value += std::stoi(r_vec[i].str);
                            break;
                        case oper_type::MINUS:
                            result_value -= std::stoi(r_vec[i].str);
                            break;
                        case oper_type::NONE:
                            emit_warning("NONE operator used in gvars "
                                    "instruction");
                            return false;
                            break;
                    }
                    return true;
                    break;
                case func_tkn_type::NUMBER:
                    emit_warning("NUMBER token before same type token in "
                            "gvars instruction");
                    return false;
                    break;
            }

            last_tkn = func_tkn_type::NUMBER;
            return true;
        };

        switch(r_vec[i].type) {
            case token_type::EQUAL:
                fatal_error("too many equal tokens in gvars instruction");
                break;
            case token_type::NUMBER:
                continue_func = number_tkn_lambda();
                last_tkn = func_tkn_type::NUMBER;
                break;
            case token_type::VARIABLE:
                r_vec[i].str = std::to_string(
                        gvars::return_value(p_gvars, r_vec[i].str));
                r_vec[i].type = token_type::NUMBER;
                continue_func = number_tkn_lambda();
                break;
            case token_type::OPERATOR:
                if(last_tkn == func_tkn_type::OPERATOR) {
                    fatal_error("OPERATOR token before same token type in "
                            "gvars instruction");
                    continue_func = false;
                }
                if(r_vec[i].str == "+") last_oper = oper_type::PLUS;
                else if(r_vec[i].str == "-") last_oper = oper_type::MINUS;
                else last_oper = oper_type::NONE;
                last_tkn = func_tkn_type::OPERATOR;
                break;
            default:
                emit_warning("wrong token type in gvars instruction");
                continue_func = false;
                break;
        }
    }

    gvars::change_val(p_gvars, r_vec[0].str, result_value);
    return true;
}

//Interpret a line which uses a function
static bool interp_func_ins(TokenVec const& r_vec,
        rooms::RoomsData_ptr const& rooms_data, Room const& room,
        parser::block_type block_type, Player& player, RoomLoopState& rls,
        RoomDisplay* room_display, game_state_s& game_state,
        std::vector<std::string>& cutscenes_vec)
{
    using namespace game_error;

    switch(r_vec[0].spec_type)
    {
        case token_spec_type::DISPLAY:
            return interp_DISPLAY_func(r_vec, room, block_type, room_display);
            break;
        case token_spec_type::SET:
            return interp_SET_func(player.gvars, r_vec);
            break;
        case token_spec_type::CUTSCENE:
            return interp_CUTSCENE_func(r_vec, cutscenes_vec);
            break;
        case token_spec_type::GO:
            return interp_GO_func(game_state, rooms_data, room, r_vec);
            break;
        case token_spec_type::GET:
            return interp_GET_func(player.inv, r_vec);
            break;
        case token_spec_type::USE:
            return interp_USE_func(player.inv, r_vec);
            break;
        case token_spec_type::GAMEOVER:
            interp_GAMEOVER_func(rls);
            break;
        case token_spec_type::EXIT:
            interp_EXIT_func(game_state);
            break;
        default:
            fatal_error("incorrect spec_token_type in function : "
                    + r_vec[0].str);
            return false;
            break;
    }

    return true;
}

//Interpret a line depending on its first token
static bool interp_ins(TokenVec const& r_vec,
        rooms::RoomsData_ptr const& rooms_data, Room const& room,
        parser::block_type block_type, Player& player, RoomLoopState& rls,
        RoomDisplay* room_display, game_state_s& game_state,
        std::vector<std::string>& cutscenes_vec)
{
    using namespace game_error;

    switch(r_vec[0].type) {
        case token_type::FUNCTION:
            return interp_func_ins(r_vec, rooms_data, room, block_type, player,
                    rls, room_display, game_state, cutscenes_vec);
            break;
        case token_type::VARIABLE:
            return interp_gvar_ins(player.gvars, r_vec);
            break;
        default:
            fatal_error("this is not yet implemented by the parser : "
                    + r_vec[0].str);
            return false;
            break;
    }
}

/* Check if the condition comparing the number of items with the specifed
 * parameter equals to true - if there is no number specified in the condition,
 * 1 is used as a value */
static bool check_HAS_condition(TokenVec r_vec,
        inventory::Inventory const& p_inv, bool& condition_result)
{
    using namespace game_error;

    int vec_size = r_vec.size();
    bool not_cond = false;
    int has_pos = 1;
    int item_pos = 2;

    if(vec_size != 4 && vec_size != 5) {
        wrg_tkn_num("HAS IF");
        return false;
    }

    if(r_vec[1].type == token_type::NOT) {
        not_cond = true;
        ++has_pos;
        ++item_pos;
    }
    if(r_vec[has_pos].type == token_type::HAS) {
        uint16_t req_item_n = 1;

        if(r_vec[item_pos].type == token_type::NUMBER) {
            req_item_n = std::stoi(r_vec[item_pos].str);
            ++item_pos;
        } if(r_vec[item_pos].type != token_type::NUMBER
                && r_vec[item_pos].type != token_type::IF
                && r_vec[item_pos].type != token_type::HAS) {
            auto item_n = inventory::return_item_n(p_inv, r_vec[item_pos].str);

            condition_result = false;

            if(item_n >= req_item_n) {
                condition_result = true;
            }
            if(not_cond) {
                condition_result = !condition_result;
            }
        } else {
            fatal_error("missing ITEM token (are the tokens in the right "
                    "order ?)");
            return false;
        }
    } else {
        fatal_error("missing HAS token (are the tokens in the right order ?)");
        return false;
    }

    return true;
}

static bool check_COMP_condition(TokenVec const& r_vec,
        gvarVector const& p_gvars, bool& condition_result)
{
    using namespace game_error;

    size_t vec_size = r_vec.size();

    if(vec_size < 4 || vec_size > 5) {
        fatal_error("wrong arg number in COMP IF");
        return false;
    } else if(r_vec[3].type == token_type::NUMBER) {
        if(r_vec[2].type == token_type::EQUAL) {
            auto varval = gvars::return_value(p_gvars, r_vec[1].str);
            int compval = std::stoi(r_vec[3].str);

            condition_result = (compval == varval);
        } else {
            fatal_error("missing equal token in COMP IF");
            return false;
        }
    } else if(r_vec[2].type == token_type::NOT
            && r_vec[3].type == token_type::EQUAL
            && r_vec[4].type == token_type::NUMBER) {
        auto varval = gvars::return_value(p_gvars, r_vec[1].str);
        int compval = std::stoi(r_vec[3].str);

        condition_result = (compval != varval);
    } else {
        fatal_error("wrong token order in COMP IF");
        return false;
    }

    return true;
}

static bool check_condition(Player const& p_player, TokenVec const& tok_vec,
        bool& condition_result)
{
    using namespace game_error;

    condition_result = true;

    if(tok_vec[2].type == token_type::EXISTS) {
        if(tok_vec.size() != 3) {
            wrg_tkn_num("EXISTS IF");
            return false;
        } else if(!gvars::exist(p_player.gvars, tok_vec[1].str)) {
            condition_result = false;
        }
    } else if(tok_vec[2].type == token_type::NOT
            && tok_vec[3].type == token_type::EXISTS) {
        if(tok_vec.size() != 4) {
            fatal_error("wrong arg number in EXISTS IF");
            return false;
        } else if(gvars::exist(p_player.gvars, tok_vec[1].str)) {
            condition_result = false;
        }
    } else if(tok_vec[1].type == token_type::VARIABLE) {
        if(!check_COMP_condition(tok_vec, p_player.gvars, condition_result)) {
            return false;
        }
    } else if(tok_vec[1].type == token_type::HAS
            || (tok_vec[1].type == token_type::NOT
            && tok_vec[2].type == token_type::HAS)) {
        if(!check_HAS_condition(tok_vec, p_player.inv, condition_result)) {
            return false;
        }
    } else {
        fatal_error("IF condition type not recognized");
        return false;
    }

    return true;
}

//Skip all lines until the end of the block
static void skip_until_end(std::vector<TokenVec> const& block_vector,
        unsigned int& i)
{
    bool is_end = false;

    while(!is_end) {
        ++i;
        if(block_vector[i][0].type == token_type::END) {
            is_end = true;
        } else if(block_vector[i][0].type == token_type::IF
                || block_vector[i][0].type == token_type::ELSE) {
            skip_until_end(block_vector, i);
        }
    }
}

//Execute instructions until the end of the block
bool parser::exec_until_end(std::vector<TokenVec> const& block_vector,
        rooms::RoomsData_ptr const& rooms_data, Room const& room,
        parser::block_type block_type, Player& player, RoomLoopState& rls,
        RoomDisplay* room_display, game_state_s& game_state,
        std::vector<std::string>& cutscenes_vec, unsigned int& i)
{
    using namespace game_error;

    bool previous_line_condition = false;
    bool prev_condition_exec = false;

    for(; i < block_vector.size(); ++i) {
        TokenVec tok_vec = block_vector[i];
        bool condition_result = false;

        token::set_runtime_tokens(tok_vec, player.gvars);

        if(game_state.should_game_exit) {
            break;
        } else if(tok_vec[0].type == token_type::END) {
            break;
        } else if(tok_vec[0].type == token_type::IF) {
            previous_line_condition = true;
            if(!check_condition(player, tok_vec, condition_result)) {
                return false;
            }

            if(condition_result) {
                ++i;
                if(!exec_until_end(block_vector, rooms_data, room, block_type,
                        player, rls, room_display, game_state, cutscenes_vec,
                        i)) {
                    return false;
                } else {
                    prev_condition_exec = true;
                }
            } else {
                skip_until_end(block_vector, i);
                prev_condition_exec = false;
            }
        } else if(tok_vec[0].type == token_type::ELSE) {
            if(!previous_line_condition) {
                fatal_error("ELSE not following an IF block");
                return false;
            }

            if(prev_condition_exec) {
                skip_until_end(block_vector, i);
            } else {
                ++i;
                if(!exec_until_end(block_vector, rooms_data, room, block_type,
                        player, rls, room_display, game_state, cutscenes_vec,
                        i)) {
                    return false;
                }
            }
            previous_line_condition = false;
        } else if(tok_vec[0].type != token_type::TEXT) {
            previous_line_condition = false;
            if(!interp_ins(tok_vec, rooms_data, room, block_type, player, rls,
                    room_display, game_state, cutscenes_vec)) {
                return false;
            }
        }
    }

    return true;
}

//Split a line into an argument and a type string
bool parser::splitline(std::string& type, std::string& arg, std::string ins)
{
    unsigned int ins_size = ins.size();
    unsigned int type_size = 0;
    bool correct_syntax = true;

    type = stringsm::getfw(ins);
    type_size = type.size();

    if(ins.size() > type.size()) {
        int starti = type_size + 1;

        for(unsigned int i = starti; i < ins_size; ++i) {
            arg += ins[i];
        }
    } else {
        correct_syntax = false;
    }
    return correct_syntax;
}
