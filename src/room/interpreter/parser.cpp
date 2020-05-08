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
#include "vars/pconst.h"
#include "vars/gvars.h"
#include "room/find.h"
#include "room/room_io.h"
#include "room/interpreter/token.h"
#include "perror.h"
#include "pstrings.h"
}

#include <cstdlib>
#include <cstring>
#include <string>
#include "parser.h"
#include "room/room.h"
#include "stringsm.h"

static void parser_execins(char* p_line, Room& currentRoom);
static bool check_condition(char* insln);
int parser_skip_until_end(int blockln);

/*Execute instructions until the end of the block*/
int parser_exec_until_end(int blockln, Room& currentRoom)
{
    bool is_end = false;
    int startln = blockln + 1;
    int endln = startln;

    for(int i = startln; !is_end; i++)
    {
        int ind = -1;
        char* buf = NULL;
        char fw[P_MAX_BUF_SIZE - 1] = {0};

        endln = i;

        roomio_fetch_ln(&buf, i);
        stringsm_getfw(fw, buf, &ind);

        if(!strcmp("END", fw)) is_end = true;
        else if(!strcmp("IF", fw)) 
        {
            if(check_condition(buf)) i = parser_exec_until_end(i, currentRoom);
            else i = parser_skip_until_end(i);
        }
        else parser_execins(buf, currentRoom);

        free(buf);
    }

    return endln;
}

/*Skip all lines until the end of the block*/
int parser_skip_until_end(int blockln)
{
    bool is_end = false;
    int startln = blockln + 1;
    int endln = startln;

    for(int i = startln; !is_end; i++)
    {
        int ind = -1;
        char* buf = NULL;
        char fw[P_MAX_BUF_SIZE - 1] = {0};

        endln = i;

        roomio_fetch_ln(&buf, i);
        stringsm_getfw(fw, buf, &ind);

        if(!strcmp("END", fw)) is_end = true;
        else if(!strcmp("IF", fw)) i = parser_skip_until_end(i);

        free(buf);
    }

    return endln;
}

static void free_TokenArr(TokenArr* p_arr);
static void interp_ins(TokenArr r_arr, Room& currentRoom);

static void parser_execins(char* p_line, Room& currentRoom)
{
    TokenArr r_arr = INIT_TKN_ARR;

    token_create_arr(&r_arr, (const char*) p_line);
    interp_ins(r_arr, currentRoom);
    free_TokenArr(&r_arr);   
}

static void free_TokenArr(TokenArr* p_arr)
{
    for(int i = 0; i < p_arr->ln; i++)
    {
        free(p_arr->list[i].str);
    }
    free(p_arr->list);
}

static void interp_func_ins(TokenArr r_arr, Room& currentRoom);

static void interp_ins(TokenArr r_arr, Room& currentRoom)
{
    switch(r_arr.list[0].type)
    {
        case FUNCTION:
            interp_func_ins(r_arr, currentRoom);
            break;
        default:
            perror_disp("this is not yet implemented by the parser", 1);
            break;
    }
}

static void interp_DISPLAY_func(Token* c_list, Room& currentRoom);
static void interp_PRINT_func(Token* c_list);
static void interp_SET_func(Token* c_list);

static void interp_func_ins(TokenArr r_arr, Room& currentRoom)
{
    if(!strcmp(r_arr.list[0].str, "DISPLAY"))
    {
        if(r_arr.ln != 2)
        {
            perror_disp("too many tokens (DISPLAY)", 1);
        }
        interp_DISPLAY_func(r_arr.list, currentRoom);
    }
    else if(!strcmp(r_arr.list[0].str, "PRINT"))
    {
        if(r_arr.ln != 2)
        {
            perror_disp("too many tokens (PRINT)", 1);
        }
        interp_PRINT_func(r_arr.list);
    }
    else if(!strcmp(r_arr.list[0].str, "SET"))
    {
        if(r_arr.ln != 4)
        {
            perror_disp("wrong number of tokens (SET)", 1);
        }
        interp_SET_func(r_arr.list);
    }
}

static bool check_COMP_condition(TokenArr* r_arr);

static bool check_condition(char* insln)
{
    bool rtrn_val = false;
    TokenArr r_arr = INIT_TKN_ARR;

    token_create_arr(&r_arr, insln);

    if(r_arr.list[2].type == EXISTS)
    {
        if(r_arr.ln != 3)
        {
            free_TokenArr(&r_arr);
            perror_disp("wrong arg number in EXISTS IF", 1);
        }
        if(gvars_exist(r_arr.list[1].str)) rtrn_val = true;

    } else if(r_arr.list[2].type == NOT && r_arr.list[3].type == EXISTS)
    {
        if(r_arr.ln != 4)
        {
            free_TokenArr(&r_arr);
            perror_disp("wrong arg number in EXISTS IF", 1);
        }
        if(!gvars_exist(r_arr.list[1].str)) rtrn_val = true;
    } else if(r_arr.list[1].type == VARIABLE)
    {
        rtrn_val = check_COMP_condition(&r_arr);
    } else 
    {
        free_TokenArr(&r_arr);
        perror_disp("IF type not recognized", 1);
    }
    
    free_TokenArr(&r_arr);
    return rtrn_val;
}

static bool check_COMP_condition(TokenArr* r_arr)
{
    bool rtrn_val = false;

    if(r_arr->ln < 4 || r_arr->ln > 5)
    {
        free_TokenArr(r_arr);
        perror_disp("wrong arg number in COMP IF", 1);
    }

    if(r_arr->list[3].type == NUMBER)
    {
        if(r_arr->list[2].type == EQUAL)
        {
            int varval = gvars_return_value(r_arr->list[1].str);
            int compval = -1;

            sscanf(r_arr->list[3].str, "%d", &compval);
            if(compval == varval) rtrn_val = true;
        } else
        {
            free_TokenArr(r_arr);
            perror_disp("missing equal token in COMP IF", 1);
        }

    } else if(r_arr->list[2].type == NOT && r_arr->list[3].type == EQUAL
            && r_arr->list[4].type == NUMBER)
    {
        int varval = gvars_return_value(r_arr->list[1].str);
        int compval = -1;

        sscanf(r_arr->list[4].str, "%d", &compval);
        if(compval != varval) rtrn_val = true;
    } else
    {
        free_TokenArr(r_arr);
        perror_disp("wrong token order in COMP IF", 1);
    }

    return rtrn_val;
}

static void interp_PRINT_func(Token* c_list)
{
    switch(c_list[1].type)
    {
        case STRING:
            {
                std::string r_str;

                stringsm_ext_str_quotes(r_str, c_list[1].str);
                printw("%s\n", r_str.c_str());
            }
            break;
        case STRING_ID:
            pstrings_display(c_list[1].str);
            printw("\n\n");
            break;
        default:
            perror_disp("token cannot be displayed (PRINT)", 0);
            break;
    }
}

static void add_all_choices(int roomln, Room& currentRoom);

static void interp_DISPLAY_func(Token* c_list, Room& currentRoom)
{
    if(!strcmp(c_list[1].str, "CHOICES"))
    {
        int roomln = -1;
        char room_name[P_MAX_BUF_SIZE - 1] = "\0";

        currentRoom.getName(room_name);

        if(currentRoom.isRoomLineSet()) roomln = currentRoom.getRoomLine();
        else
        {
            find_roomline(room_name, &roomln);
            currentRoom.setRoomLine(roomln);
        }
        add_all_choices(roomln, currentRoom);
    } else
    {
        perror_disp("displaying one choice is not yet implemented", 0);
    }
}

static void interp_SET_func(Token* c_list)
{
    int val;

    if(gvars_exist(c_list[1].str))
    {
        perror_disp("gvar already exist", 1);
    }
    if(c_list[2].type != EQUAL)
    {
        perror_disp("missing EQUAL token (SET)", 1);
    }
    if(c_list[3].type != NUMBER)
    {
        perror_disp("no value assigned to var during its init", 1);
    }

    sscanf(c_list[3].str, "%d", &val);
    gvars_set_var(c_list[1].str, val);
}

/*Extract the type and the argument from a string*/
void parser_splitline(char* type, char* arg, char* ins)
{
    int i = 0;
    int len = 0;
    char argtocpy[P_MAX_BUF_SIZE] = {0};
    
    stringsm_chomp(ins);
    len = strlen(ins);
    ins[len] = '\0';
    stringsm_getfw(type, ins, &i);
    if(len != (int)strlen(type))
    {
        int findex = 0;

        for(int index = i; index < len; index++)
        {
            argtocpy[findex] = ins[index];
            findex++;
        }
        strcpy(arg, argtocpy);
    } else
    {
        strcpy(arg, "ARGNULL");
    }
}

static void add_all_choices(int roomln, Room& currentRoom)
{
    int choicesln = 0;
    bool choicesremain = true;
    bool choicesexist = false;
    
    if(currentRoom.isChoicesLineSet())
    {
        choicesexist = true;
        choicesln = currentRoom.getChoicesLine();
    } else
    {
        choicesexist = find_choicesline(&choicesln, roomln);

        if(!choicesexist) perror_disp("Missing CHOICES block", true);
        else currentRoom.setChoicesLine(choicesln);
    }
    for(int i = 1; choicesremain; i++)
    {
        int onechoiceln = 0;
        bool choicesexist = find_onechoiceline((i), choicesln, &onechoiceln);

        if(!choicesexist)
        {
            if(i == 1) perror_disp("No CHOICE block found", true);
            choicesremain = false;
        } else
        {
            currentRoom.addDisplayChoice(onechoiceln);
        }
    }
}
