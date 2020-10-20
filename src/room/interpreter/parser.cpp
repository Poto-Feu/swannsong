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

#include "room/interpreter/parser.hpp"
#include "room/interpreter/token.hpp"
#include "vars/gvars.hpp"
#include "cutscenes.hpp"
#include "game_error.hpp"
#include "player/inventory.hpp"
#include "pcurses.hpp"
#include "stringsm.h"

namespace parser
{
    using namespace game_error;

    static void CHOICE_block_err(std::string const& func_name)
    {
        fatal_error(func_name + " function cannot be used in CHOICE block");
    }

    static void wrg_tkn_num(std::string const& func_name)
    {
        fatal_error("wrong number of tokens (" + func_name + ")");
    }

    //Interpret a line which use the SET function
    static void interp_SET_func(TokenVec r_vec, gvarVector& p_gvars)
    {
        if(r_vec.size() != 4) {
            wrg_tkn_num("SET");
            return;
        } else {
            if(gvars::exist(p_gvars, r_vec[1].str)) fatal_error("gvar already exist");
            else if(r_vec[2].type != token_type::EQUAL) fatal_error("missing EQUAL token (SET)");
            else if(r_vec[3].type != token_type::NUMBER) {
                fatal_error("no value assigned to var during its init");
            } else gvars::set_var(p_gvars, r_vec[1].str, std::stoi(r_vec[3].str));
        }
    }

    //Interpret a line which use the DISPLAY function
    static void interp_DISPLAY_func(TokenVec r_vec, room_struct& p_struct)
    {
        if(r_vec.size() != 2 && r_vec.size() != 3) {
            wrg_tkn_num("DISPLAY");
            return;
        }

        if(r_vec.size() == 3) {
            if(r_vec[1].str != "CHOICE") {
                game_error::fatal_error("incorrect DISPLAY syntax (" + p_struct.currRoom.getName()
                        + ")");
            } else if(r_vec[2].str.length() < 3 && stringsm::is_number(r_vec[2].str)) {
                unsigned int choice_n = stoi(r_vec[2].str);

                if(!p_struct.currRoom.isChoicePresent(choice_n)) {
                    game_error::fatal_error("choice " + r_vec[2].str + " doesn't exist in " +
                            p_struct.currRoom.getName() + " ROOM");
                } else p_struct.currState.addChoice(stoi(r_vec[2].str));
            }
        } else if(r_vec.size() == 2) {
            if(p_struct.currState.getBlockType() == RoomState::bt::CHOICE) {
                CHOICE_block_err("DISPLAY");
            } else if(r_vec[1].str == "CHOICES") p_struct.currState.addAllChoices();
            else if(r_vec[1].str == "TITLE") p_struct.currState.addTitle();
            else if(r_vec[1].str == "DESC") p_struct.currState.addDesc();
            else if(r_vec[1].str == "ALL") {
                p_struct.currState.addTitle();
                p_struct.currState.addDesc();
                p_struct.currState.addAllChoices();
            }
        }
    }

    //Interpret a line which use the PRINT function
    static void interp_PRINT_func(TokenVec const& r_vec, RoomState& p_state,
            PStrings const& program_strings)
    {
        if(r_vec.size() != 2) wrg_tkn_num("PRINT");
        else {
            if(p_state.getBlockType() == RoomState::bt::CHOICE) CHOICE_block_err("PRINT");

            switch(r_vec[1].type) {
                case token_type::STRING:
                    p_state.addString(r_vec[1].str);
                    break;
                case token_type::STRING_ID:
                    p_state.addString(program_strings.fetch(r_vec[1].str));
                    break;
                default:
                    emit_warning("token cannot be displayed (PRINT)");
                    break;
            }
        }
    }

    //Interpret a line which use the CUTSCENE function
    static void interp_CUTSCENE_func(TokenVec const& r_vec, RoomState& p_state)
    {
        if(r_vec.size() != 2) wrg_tkn_num("CUTSCENE");
        else {
            if(cutscenes::check_exist(r_vec[1].str)) p_state.addCutscene(r_vec[1].str);
            else emit_warning("unknown CUTSCENE id (" + r_vec[1].str + ")");
        }
    }

    //Intepret a line which use the GO function
    static void interp_GO_func(TokenVec const& r_vec, room_struct& p_struct)
    {
        if(r_vec.size() != 2) wrg_tkn_num("GO");
        else {
            if(p_struct.roomMap.find(r_vec[1].str) != p_struct.roomMap.cend()) {
                p_struct.currLoopState.setNextRoom(r_vec[1].str);
            } else fatal_error(r_vec[1].str + " ROOM does not exist (GO function in "
                    + p_struct.currRoom.getName() + " ROOM)");
        }
    }

    /*Interpret a line which use the UNFINISHED function, which tells the player they have reached
    an unfinished part of the program*/
    static void interp_UNFINISHED_func(TokenVec const& r_vec, RoomLoopState& p_rls)
    {
        if(r_vec.size() != 1) {
            wrg_tkn_num("UNFINISHED");
        } else p_rls.setUnfinished();
       
    }

    //Interpret a line which use the GET function, which add an item to the player's inventory
    static void interp_GET_func(TokenVec const& p_vec, inventory::Inventory& p_inv)
    {
        using namespace inventory;
        int item_name_pos = 1;
        item_val_type item_n = 1;

        if(p_vec.size() == 3) {
            if(p_vec[1].type == token_type::NUMBER) {
                item_name_pos = 2;
                item_n = std::stoi(p_vec[1].str);
            } else {
                fatal_error("second part of a GET instruction must be a NUMBER or an ITEM");
                return;
            }
        } else if(p_vec.size() != 2) {
            wrg_tkn_num("GET");
            return;
        }

        inventory::getitem(p_inv, p_vec[item_name_pos].str, item_n);
    }

    static void interp_USE_func(TokenVec const& p_vec, inventory::Inventory& p_inv)
    {
        using namespace inventory;

        int item_name_pos = 1;
        item_val_type item_n = 1;

        if(p_vec.size() == 3) {
            if(p_vec[1].type == token_type::NUMBER) {
                item_name_pos = 2;
                item_n = std::stoi(p_vec[1].str);
            } else {
                fatal_error("second part of an USE instruction must be a NUMBER or an ITEM");
                return;
            }
        } else if(p_vec.size() != 2) {
            wrg_tkn_num("USE");
            return;
        }

        inventory::useitem(p_inv, p_vec[item_name_pos].str, item_n);
    }

    static void interp_EXIT_func(RoomLoopState& p_rls)
    {
        p_rls.endLoop();
    }

    //Interpret a line which changes a gvar value
    static void interp_gvar_ins(TokenVec r_vec, gvarVector& p_gvars)
    {
        if(r_vec[1].type != token_type::EQUAL) {
            fatal_error("second token of a gvars instruction must be an equal sign");
            return;
        } else if(r_vec.size() == 3 && r_vec[2].type == token_type::NUMBER) {
            gvars::change_val(p_gvars, r_vec[0].str, std::stoi(r_vec[2].str));
        }
        
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

        for(unsigned int i = 2; i < r_vec.size() && continue_func; ++i) {
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
                                emit_warning("NONE operator used in gvars instruction");
                                return false;
                                break;
                        }
                        return true;
                        break;
                    case func_tkn_type::NUMBER:
                        emit_warning("NUMBER token before same type token in gvars instruction");
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
                    r_vec[i].str = std::to_string(gvars::return_value(p_gvars, r_vec[i].str));
                    r_vec[i].type = token_type::NUMBER;
                    continue_func = number_tkn_lambda();
                    break;
                case token_type::OPERATOR:
                    if(last_tkn == func_tkn_type::OPERATOR) {
                        fatal_error("OPERATOR token before same token type in gvars instruction");
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
    }

    //Interpret a line which uses a function
    static void interp_func_ins(TokenVec const& r_vec, room_struct& p_struct)
    {
        switch(r_vec[0].spec_type)
        {
            case token_spec_type::DISPLAY:
                interp_DISPLAY_func(r_vec, p_struct);
                break;
            case token_spec_type::PRINT:
                interp_PRINT_func(r_vec, p_struct.currState, p_struct.program_strings);
                break;
            case token_spec_type::SET:
                interp_SET_func(r_vec, p_struct.currPlayer.gvars);
                break;
            case token_spec_type::CUTSCENE:
                interp_CUTSCENE_func(r_vec, p_struct.currState);
                break;
            case token_spec_type::GO:
                interp_GO_func(r_vec, p_struct);
                break;
            case token_spec_type::UNFINISHED:
                interp_UNFINISHED_func(r_vec, p_struct.currLoopState);
                break;
            case token_spec_type::GET:
                interp_GET_func(r_vec, p_struct.currPlayer.inv);
                break;
            case token_spec_type::USE:
                interp_USE_func(r_vec, p_struct.currPlayer.inv);
                break;
            case token_spec_type::EXIT:
                interp_EXIT_func(p_struct.currLoopState);
                break;
            default:
                fatal_error("incorrect spec_token_type in function : " + r_vec[0].str);
                break;
        }
    }

    //Interpret a line depending on its first token
    static void interp_ins(TokenVec const& r_vec, room_struct& p_struct)
    {
        switch(r_vec[0].type) {
            case token_type::FUNCTION:
                interp_func_ins(r_vec, p_struct);
                break;
            case token_type::VARIABLE:
                interp_gvar_ins(r_vec, p_struct.currPlayer.gvars);
                break;
            default:
                fatal_error("this is not yet implemented by the parser : "
                        + r_vec[0].str);
                break;
        }
    }

    /*Check if the condition comparing the number of items with the specifed parameter equals to
    true - if there is no number specified in the condition, 1 is used as a value*/
    static bool check_HAS_condition(TokenVec r_vec, inventory::Inventory const& p_inv)
    {
        int vec_size = r_vec.size();

        if(vec_size == 4 || vec_size == 5) {
            bool not_cond = false;
            int has_pos = 1;
            int item_pos = 2;

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
                    bool rtrn_val = false;
                    auto item_n = inventory::return_item_n(p_inv, r_vec[item_pos].str);

                    if(item_n >= req_item_n) rtrn_val = true;
                    if(not_cond) rtrn_val = !rtrn_val;

                    return rtrn_val;
                } else fatal_error("missing ITEM token (are the tokens in the right order ?)");
            } else fatal_error("missing HAS token (are the tokens in the right order ?)");
        } else wrg_tkn_num("HAS IF");
        return false;
    }

    static bool check_COMP_condition(TokenVec const& r_vec, gvarVector const& p_gvars)
    {
        size_t vec_size = r_vec.size();

        if(vec_size < 4 || vec_size > 5) {
            fatal_error("wrong arg number in COMP IF");
        } else if(r_vec[3].type == token_type::NUMBER) {
            if(r_vec[2].type == token_type::EQUAL) {
                auto varval = gvars::return_value(p_gvars, r_vec[1].str);
                int compval = std::stoi(r_vec[3].str);

                if(compval == varval) return true;
            } else fatal_error("missing equal token in COMP IF");
        } else if(r_vec[2].type == token_type::NOT
                && r_vec[3].type == token_type::EQUAL
                && r_vec[4].type == token_type::NUMBER) {
            auto varval = gvars::return_value(p_gvars, r_vec[1].str);
            int compval = std::stoi(r_vec[3].str);

            if(compval != varval) return true;
        } else fatal_error("wrong token order in COMP IF");
        return false;
    }

    //Split a line into an argument and a type string
    bool splitline(std::string& type, std::string& arg, std::string ins)
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
        } else correct_syntax = false;
        return correct_syntax;
    }

    //Skip all lines until the end of the block
    void skip_until_end(std::vector<TokenVec> const& block_vector, unsigned int& i)
    {
        bool is_end = false;

        for(++i; !is_end;) {
            ++i;
            if(block_vector[i][0].type == token_type::END) is_end = true;
            else if(block_vector[i][0].type == token_type::IF) skip_until_end(block_vector, i);
        }
    }

    static bool check_condition(TokenVec const& current_line, Player const& p_player)
    {
        if(current_line[2].type == token_type::EXISTS) {
            if(current_line.size() != 3) wrg_tkn_num("EXISTS IF");
            else if(gvars::exist(p_player.gvars, current_line[1].str)) return true;
        } else if(current_line[2].type == token_type::NOT
                && current_line[3].type == token_type::EXISTS) {
            if(current_line.size() != 4) fatal_error("wrong arg number in EXISTS IF");
            else if(!gvars::exist(p_player.gvars, current_line[1].str)) return true;
        } else if(current_line[1].type == token_type::VARIABLE) {
            return check_COMP_condition(current_line, p_player.gvars);
        } else if(current_line[1].type == token_type::HAS
                || (current_line[1].type == token_type::NOT
                && current_line[2].type == token_type::HAS)) {
            return check_HAS_condition(current_line, p_player.inv);
        } else fatal_error("IF condition type not recognized");
        return false;
    }

    //Execute instructions until the end of the block
    void exec_until_end(std::vector<TokenVec> const& block_vector, room_struct& p_struct,
            unsigned int& i)
    {
        bool previous_line_condition = false;
        bool prev_condition_exec = false;

        for(; !p_struct.currLoopState.is_unfinished() && i < block_vector.size(); ++i) {
            TokenVec current_line = block_vector[i];

            token::set_runtime_tokens(current_line, p_struct.currPlayer.gvars);

            if(p_struct.currLoopState.is_endgame()) break;
            else if(current_line[0].type == token_type::END) break;
            else if(current_line[0].type == token_type::IF) {
                previous_line_condition = true;
                if(check_condition(current_line, p_struct.currPlayer)) {
                    ++i;
                    exec_until_end(block_vector, p_struct, i);
                    prev_condition_exec = true;
                } else {
                    if(has_encountered_fatal()) break;

                    parser::skip_until_end(block_vector, i);
                    prev_condition_exec = false;
                }
            } else if(current_line[0].type == token_type::ELSE) {
                if(!previous_line_condition) {
                    game_error::fatal_error("ELSE not following an IF block");
                    return;
                }

                if(prev_condition_exec) parser::skip_until_end(block_vector, i);
                else {
                    ++i;
                    exec_until_end(block_vector, p_struct, i);
                }
                previous_line_condition = false;
            } else if(current_line[0].type != token_type::TEXT) {
                previous_line_condition = false;
                interp_ins(current_line, p_struct);
                if(has_encountered_fatal()) break;
            }
        }
    }
}
