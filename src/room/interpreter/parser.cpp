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

extern "C" {
#include <curses.h>
#include "perror.h"
}

#include "parser.hpp"
#include "room/interpreter/token.hpp"
#include "room/room_io.h"
#include "room/find.hpp"
#include "vars/gvars.hpp"
#include "inventory.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "stringsm.h"

static void CHOICE_block_err(std::string const& func_name)
{
    std::string err_msg = func_name 
        + " function cannot be used in CHOICE block";
    perror_disp(err_msg.c_str(), true);
}

static void wrg_tkn_num(std::string const& func_name)
{
    std::string err_msg = "wrong number of tokens (" + func_name + ")";
    perror_disp(err_msg.c_str(), true);
}

//Add all room choices into a vector in the RoomManager
static void add_all_choices(int roomln, Room& currentRoom,
        RoomManager& p_roomman)
{
    int choicesln = 0;
    bool choicesremain = true;
    
    if(currentRoom.isChoicesLineSet()) {
        choicesln = currentRoom.getChoicesLine();
    } else {
        bool choicesexist = room_find::choicesline(choicesln, roomln);

        if(!choicesexist) perror_disp("Missing CHOICES block", true);
        else currentRoom.setChoicesLine(choicesln);
    }

    for(int i = 1; choicesremain; i++) {
        int onechoiceln = 0;
        bool choiceexist = room_find::onechoiceline(i, choicesln,
                onechoiceln);

        if(!choiceexist) {
            if(i == 1) perror_disp("No CHOICE block found", true);
            choicesremain = false;
        } else p_roomman.addChoice(Choice(i, onechoiceln));
    }
}

//Interpret a line which use the SET function
static void interp_SET_func(TokenVec r_vec)
{
    int val = -1;

    if(gvars::exist(r_vec[1].str)) perror_disp("gvar already exist", true);
    if(r_vec[2].type != token_type::EQUAL) {
        perror_disp("missing EQUAL token (SET)", true);
    } if(r_vec[3].type != token_type::NUMBER) {
        perror_disp("no value assigned to var during its init", true);
    }

    val = std::stoi(r_vec[3].str);
    gvars::set_var(r_vec[1].str, val);
}

//Interpret a line which use the DISPLAY function
static void interp_DISPLAY_func(TokenVec r_vec, Room& currentRoom,
        RoomManager& p_roomman)
{
    if(p_roomman.getBlockType() == RoomManager::bt::CHOICE) {
        CHOICE_block_err("DISPLAY");
    }

    if(r_vec[1].str == "CHOICES") {
        int roomln = -1;

        roomln = currentRoom.getRoomLine();
        add_all_choices(roomln, currentRoom, p_roomman);
    } else if(r_vec[1].str == "TITLE") p_roomman.addTitle();
    else if(r_vec[1].str == "DESC") p_roomman.addDesc();
    else perror_disp("displaying one choice is not yet implemented", false);
}

//Interpret a line which use the PRINT function
static void interp_PRINT_func(TokenVec r_vec, RoomManager& p_roomman)
{
    if(p_roomman.getBlockType() == RoomManager::bt::CHOICE) {
        CHOICE_block_err("PRINT");
    }

    switch(r_vec[1].type) {
        case token_type::STRING:
            p_roomman.addString(r_vec[1].str);
            break;
        case token_type::STRING_ID:
            p_roomman.addString(pstrings::fetch(r_vec[1].str));
            break;
        default:
            perror_disp("token cannot be displayed (PRINT)", 0);
            break;
    }
}

//Interpret a line which use the CUTSCENE function
static void interp_CUTSCENE_func(TokenVec r_vec, RoomManager& p_roomman)
{
    if(cutscenes::check_exist(r_vec[1].str)) {
        p_roomman.addCutscene(r_vec[1].str);
    } else {
        std::string err_str = "unknown CUTSCENE id (" + r_vec[1].str + ")";
        perror_disp(err_str.c_str(), false);
    }
}

//Intepret a line which use the GO function
static void interp_GO_func(TokenVec r_vec, RoomManager& p_roomman)
{
    int roomln = 0;

    if(!room_find::roomline(&roomln, r_vec[1].str)) {
        std::string err_msg =  "\"" + r_vec[1].str + "\" room was not found";

        perror_disp(err_msg.c_str(), true);
    } else p_roomman.setNextRoom(r_vec[1].str);
}

/*Interpret a line which use the UNFINISHED function, which tells the player
they have reached an unfinished part of the program*/
static void interp_UNFINISHED_func(RoomManager& p_rmm)
{
    p_rmm.endLoop();

    clear();
    move(3, pcurses::margin);
    pcurses::display_center_string(pstrings::fetch("unfinished_str"));
}

/*Interpret a line which use the GET function, which add an item to the player's
inventory*/
static void interp_GET_func(TokenVec const& p_vec)
{
    int item_name_pos = 1;
    unsigned int item_n = 1;

    if (p_vec.size() == 3) {
        if(p_vec[1].type == token_type::NUMBER) {
            item_name_pos = 2;
            item_n = std::stoi(p_vec[1].str);
        } else {
            perror_disp(
                "second part of a GET instruction must be a NUMBER or an ITEM",
                true);
        }
    } else if(p_vec.size() != 2) wrg_tkn_num("GET");

    inventory::player_getitem(p_vec[item_name_pos].str, item_n);
}

static void interp_USE_func(TokenVec const& p_vec)
{
    int item_name_pos = 1;
    unsigned int item_n = 1;

    if (p_vec.size() == 3) {
        if(p_vec[1].type == token_type::NUMBER) {
            item_name_pos = 2;
            item_n = std::stoi(p_vec[1].str);
        } else {
            perror_disp(
                "second part of an USE instruction must be a NUMBER or an ITEM",
                true);
        }
    } else if(p_vec.size() != 2) wrg_tkn_num("USE");

    inventory::player_useitem(p_vec[item_name_pos].str, item_n);
}

//Interpret a line which use a function
static void interp_func_ins(TokenVec r_vec, Room& currentRoom,
        RoomManager& p_roomman)
{
    if(r_vec[0].str == "DISPLAY") {
        if(r_vec.size() != 2) wrg_tkn_num("DISPLAY");
        else interp_DISPLAY_func(r_vec, currentRoom, p_roomman);
    } else if(r_vec[0].str == "PRINT") {
        if(r_vec.size() != 2) wrg_tkn_num("PRINT");
        else interp_PRINT_func(r_vec, p_roomman);
    } else if(r_vec[0].str == "SET") {
        if(r_vec.size() != 4) wrg_tkn_num("SET");
        else interp_SET_func(r_vec);
    } else if(r_vec[0].str == "CUTSCENE") {
        if(r_vec.size() != 2) wrg_tkn_num("CUTSCENE");
        else interp_CUTSCENE_func(r_vec, p_roomman);
    } else if(r_vec[0].str == "GO") {
        if(r_vec.size() != 2) wrg_tkn_num("GO");
        else interp_GO_func(r_vec, p_roomman);
    } else if(r_vec[0].str == "UNFINISHED") {
        if(r_vec.size() != 1) wrg_tkn_num("UNFINISHED");
        else interp_UNFINISHED_func(p_roomman);
    } else if(r_vec[0].str == "GET") interp_GET_func(r_vec);
    else if(r_vec[0].str == "USE") interp_USE_func(r_vec);
}

//Interpret a line depending on its first token
static void interp_ins(TokenVec r_vec, Room& currentRoom,
        RoomManager& p_roomman)
{
    switch(r_vec[0].type) {
        case token_type::FUNCTION:
            interp_func_ins(r_vec, currentRoom, p_roomman);
            break;
        default:
            perror_disp("this is not yet implemented by the parser", true);
            break;
    }
}

static void parser_execins(std::string p_line, Room& currentRoom,
        RoomManager& p_roomman)
{
    TokenVec r_vec = token::create_arr(p_line);
    interp_ins(r_vec, currentRoom, p_roomman);
}

static bool check_COMP_condition(TokenVec r_vec)
{
    int vec_size = r_vec.size();

    if(vec_size < 4 || vec_size > 5) {
        perror_disp("wrong arg number in COMP IF", true);
    } else if(r_vec[3].type == token_type::NUMBER) {
        if(r_vec[2].type == token_type::EQUAL) {
            int varval = gvars::return_value(r_vec[1].str);
            int compval = std::stoi(r_vec[3].str);

            if(compval == varval) return true;
        } else perror_disp("missing equal token in COMP IF", true);
    } else if(r_vec[2].type == token_type::NOT
            && r_vec[3].type == token_type::EQUAL
            && r_vec[4].type == token_type::NUMBER) {
        int varval = gvars::return_value(r_vec[1].str);
        int compval = std::stoi(r_vec[3].str);

        if(compval != varval) return true;
    } else perror_disp("wrong token order in COMP IF", true);

    return false;
}

static bool check_HAS_condition(TokenVec r_vec)
{
    bool not_cond = false;
    int vec_size = r_vec.size();
    unsigned int req_item_n = 1;
    int has_pos = 1;
    int item_pos = 2;

    if(vec_size == 4 || vec_size == 5) {
        if(r_vec[1].type == token_type::NOT) {
            not_cond = true;
            ++has_pos;
            ++item_pos;
        }

        if(r_vec[has_pos].type == token_type::HAS) {
            if(r_vec[item_pos].type == token_type::NUMBER) {
                req_item_n = std::stoi(r_vec[item_pos].str);
                ++item_pos;
            }

            if(r_vec[item_pos].type != token_type::NUMBER
                    && r_vec[item_pos].type != token_type::IF
                    && r_vec[item_pos].type != token_type::HAS) {
                bool rtrn_val = false;
                auto item_n = inventory::return_item_n(r_vec[item_pos].str);

                if(item_n >= req_item_n) rtrn_val = true;
                if(not_cond) rtrn_val = !rtrn_val;

                return rtrn_val;
            } else perror_disp(
                    "missing ITEM token (are the tokens in the right order ?)",
                    true);
        } else perror_disp(
            "missing HAS token (are the tokens in the right order ?)", true);

    } else wrg_tkn_num("HAS IF");

    return false;
}

static bool check_condition(std::string insln)
{
    TokenVec r_vec = token::create_arr(insln);

    if(r_vec[2].type == token_type::EXISTS) {
        if(r_vec.size() != 3) {
            wrg_tkn_num("EXISTS IF");
        } else if(gvars::exist(r_vec[1].str)) return true;
    } else if(r_vec[2].type == token_type::NOT
            && r_vec[3].type == token_type::EXISTS) {
        if(r_vec.size() != 4) {
            perror_disp("wrong arg number in EXISTS IF", true);
        } else if(!gvars::exist(r_vec[1].str)) return true;
    } else if(r_vec[1].type == token_type::VARIABLE) {
        return check_COMP_condition(r_vec);
    } else if(r_vec[1].type == token_type::HAS
            || (r_vec[1].type == token_type::NOT
             && r_vec[2].type == token_type::HAS)) {
        return check_HAS_condition(r_vec);
    } else perror_disp("IF condition type not recognized", true);

    return false;
}

namespace parser
{
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
                if(ins.at(i) == ' ' || ins.at(i) == '\t') break;
                else arg += ins.at(i);
            }
        } else correct_syntax = false;
        return correct_syntax;
    }

    //Skip all lines until the end of the block
    int skip_until_end(int blockln)
    {
        bool is_end = false;
        int startln = blockln + 1;
        int endln = startln;

        for(int i = startln; !is_end; i++) {
            std::string fw;
            std::string buf;

            endln = i;
            roomio::fetch_ln(buf, i);
            fw = stringsm::getfw(buf);

            if(fw == "END") is_end = true;
            else if(fw == "IF") i = skip_until_end(i);
        }
        return endln;
    }

    //Execute instructions until the end of the block
    int exec_until_end(int blockln, Room& currentRoom, RoomManager &p_roomman)
    {
        bool is_end = false;
        int startln = blockln + 1;
        int endln = startln;

        for(int i = startln; !is_end; i++) {
            std::string buf;
            std::string fw;

            if(p_roomman.is_endgame()) break;

            endln = i;
            roomio::fetch_ln(buf, i);
            fw = stringsm::getfw(buf);

            if(fw == "END") is_end = true;
            else if(fw == "IF") {
                if(check_condition(buf)) {
                    i = exec_until_end(i, currentRoom, p_roomman);
                }
                else i = parser::skip_until_end(i);
            } else parser_execins(buf, currentRoom, p_roomman);
        }
        return endln;
    }
}
