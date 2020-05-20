/*
    Copyright (C) 2020 Adrien Saad

    This file is part of SwannSong.

    SwannSong is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SwannSong is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong.  If not, see <https://www.gnu.org/licenses/>.
*/

extern "C" {
#include <curses.h>
#include "perror.h"
}

#include <string>
#include "parser.hpp"
#include "room/interpreter/token.hpp"
#include "room/room.hpp"
#include "room/room_io.h"
#include "room/find.hpp"
#include "vars/gvars.hpp"
#include "vars/pconst.hpp"
#include "pstrings.h"
#include "stringsm.h"

/*Add all room choices into a vector in the DisplayManager*/
static void add_all_choices(int roomln, Room& currentRoom,
        DisplayManager& p_dispm)
{
    int choicesln = 0;
    bool choicesremain = true;
    
    if(currentRoom.isChoicesLineSet())
    {
        choicesln = currentRoom.getChoicesLine();
    } else
    {
        bool choicesexist = room_find::choicesline(choicesln, roomln);

        if(!choicesexist) perror_disp("Missing CHOICES block", true);
        else currentRoom.setChoicesLine(choicesln);
    }

    for(int i = 1; choicesremain; i++)
    {
        int onechoiceln = 0;
        bool choiceexist = room_find::onechoiceline(i, choicesln,
                onechoiceln);

        if(!choiceexist)
        {
            if(i == 1) perror_disp("No CHOICE block found", true);
            choicesremain = false;
        } else p_dispm.addChoice(Choice(i, onechoiceln));
    }
}

/*Interpret a line which use the SET function*/
static void interp_SET_func(TokenVec r_vec)
{
    int val = -1;

    if(gvars::exist(r_vec[1].str)) perror_disp("gvar already exist", true);
    if(r_vec[2].type != EQUAL)
    {
        perror_disp("missing EQUAL token (SET)", 1);
    }
    if(r_vec[3].type != NUMBER)
    {
        perror_disp("no value assigned to var during its init", 1);
    }

    val = std::stoi(r_vec[3].str);
    gvars::set_var(r_vec[1].str, val);
}

/*Interpret a line which use the DISPLAY function*/
static void interp_DISPLAY_func(TokenVec r_vec, Room& currentRoom,
        DisplayManager& p_dispm)
{
    if(r_vec[1].str == "CHOICES")
    {
        int roomln = -1;

        roomln = currentRoom.getRoomLine();
        add_all_choices(roomln, currentRoom, p_dispm);
    } else if(r_vec[1].str == "TITLE") p_dispm.addTitle();
    else if(r_vec[1].str == "DESC") p_dispm.addDesc();
    else
    {
        perror_disp("displaying one choice is not yet implemented", false);
    }
}

/*Interpret a line which use the PRINT function*/
static void interp_PRINT_func(TokenVec r_vec)
{
    switch(r_vec[1].type)
    {
        case STRING:
            {
                std::string r_str = stringsm::ext_str_quotes(r_vec[1].str);
                printw("%s\n", r_str.c_str());
            }
            break;
        case STRING_ID:
            pstrings::display(r_vec[1].str);
            printw("\n\n");
            break;
        default:
            perror_disp("token cannot be displayed (PRINT)", 0);
            break;
    }
}

/*Interpret a line which use a function*/
static void interp_func_ins(TokenVec r_vec, Room& currentRoom,
        DisplayManager& p_dispm)
{
    if(r_vec[0].str == "DISPLAY")
    {
        if(r_vec.size() != 2) perror_disp("too many tokens (DISPLAY)", true);
        interp_DISPLAY_func(r_vec, currentRoom, p_dispm);
    }
    else if(r_vec[0].str == "PRINT")
    {
        if(r_vec.size() != 2)
        {
            perror_disp("too many tokens (PRINT)", true);
        }
        interp_PRINT_func(r_vec);
    }
    else if(r_vec[0].str == "SET")
    {
        if(r_vec.size() != 4)
        {
            perror_disp("wrong number of tokens (SET)", true);
        }
        interp_SET_func(r_vec);
    }
}

/*Interpret a line depending on its first token*/
static void interp_ins(TokenVec r_vec, Room& currentRoom,
        DisplayManager& p_dispm)
{
    switch(r_vec[0].type)
    {
        case FUNCTION:
            interp_func_ins(r_vec, currentRoom, p_dispm);
            break;
        default:
            perror_disp("this is not yet implemented by the parser", true);
            break;
    }
}

static void parser_execins(std::string p_line, Room& currentRoom,
        DisplayManager& p_dispm)
{
    TokenVec r_vec = token::create_arr(p_line);
    interp_ins(r_vec, currentRoom, p_dispm);
}

static bool check_COMP_condition(TokenVec r_vec)
{
    bool rtrn_val = false;
    int vec_size = r_vec.size();

    if(vec_size < 4 || vec_size > 5)
    {
        perror_disp("wrong arg number in COMP IF", true);
    } else if(r_vec[3].type == NUMBER)
    {
        if(r_vec[2].type == EQUAL)
        {
            int varval = gvars::return_value(r_vec[1].str);
            int compval = std::stoi(r_vec[3].str);

            if(compval == varval) rtrn_val = true;
        } else perror_disp("missing equal token in COMP IF", true);
    } else if(r_vec[2].type == NOT && r_vec[3].type == EQUAL
            && r_vec[4].type == NUMBER)
    {
        int varval = gvars::return_value(r_vec[1].str);
        int compval = std::stoi(r_vec[3].str);

        if(compval != varval) rtrn_val = true;
    } else perror_disp("wrong token order in COMP IF", true);

    return rtrn_val;
}

static bool check_condition(std::string insln)
{
    bool rtrn_val = false;
    TokenVec r_vec = token::create_arr(insln);

    if(r_vec[2].type == EXISTS)
    {
        if(r_vec.size() != 3)
        {
            perror_disp("wrong arg number in EXISTS IF", true);
        } else if(gvars::exist(r_vec[1].str))
        {
            rtrn_val = true;
        }

    } else if(r_vec[2].type == NOT && r_vec[3].type == EXISTS)
    {
        if(r_vec.size() != 4)
        {
            perror_disp("wrong arg number in EXISTS IF", true);
        } else if(!gvars::exist(r_vec[1].str)) rtrn_val = true;
    } else if(r_vec[1].type == VARIABLE)
    {
        rtrn_val = check_COMP_condition(r_vec);
    } else perror_disp("IF type not recognized", true);

    return rtrn_val;
}

namespace parser
{
    /*Split a line into an argument and a type string*/
    bool splitline(std::string& type, std::string& arg, std::string ins)
    {
        unsigned int ins_size = ins.size();
        unsigned int type_size = 0;
        bool correct_syntax = true;

        type = stringsm::getfw(ins);
        type_size = type.size();

        if(ins.size() > type.size())
        {
            int starti = type_size + 1;

            for(unsigned int i = starti; i < ins_size; ++i)
            {
                if(ins.at(i) == ' ' || ins.at(i) == '\t') break;
                else arg += ins.at(i);
            }
        } else correct_syntax = false;

        return correct_syntax;
    }

    /*Skip all lines until the end of the block*/
    int skip_until_end(int blockln)
    {
        bool is_end = false;
        int startln = blockln + 1;
        int endln = startln;

        for(int i = startln; !is_end; i++)
        {
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

    /*Execute instructions until the end of the block*/
    int exec_until_end(int blockln, Room& currentRoom, DisplayManager &p_dispm)
    {
        bool is_end = false;
        int startln = blockln + 1;
        int endln = startln;

        for(int i = startln; !is_end; i++)
        {
            std::string buf;
            std::string fw;

            endln = i;

            roomio::fetch_ln(buf, i);
            fw = stringsm::getfw(buf);

            if(fw == "END") is_end = true;
            else if(fw == "IF")
            {
                if(check_condition(buf.c_str()))
                {
                    i = exec_until_end(i, currentRoom, p_dispm);
                }
                else i = parser::skip_until_end(i);
            }
            else parser_execins(buf, currentRoom, p_dispm);
        }
        return endln;
    }
}
