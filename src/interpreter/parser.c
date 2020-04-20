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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"
#include "vars/pconst.h"
#include "vars/pvars.h"
#include "vars/gvars.h"
#include "room/room.h"
#include "room/find.h"
#include "room/room_io.h"
#include "interpreter/token.h"
#include "fileio/fileio.h"
#include "perror.h"
#include "pstrings.h"
#include "stringsm.h"

static void parser_execins(char* p_line);

int parser_exec_until_end(int blockln)
{
    bool is_end = false;
    int startln = blockln + 1;
    int endln = startln;

    for(int i = startln; !is_end; i++)
    {
        int ind = -1;
        char* buf = NULL;
        char* fw = calloc((P_MAX_BUF_SIZE - 1), sizeof(char));

        endln = i;
        roomio_fetch_ln(&buf, i);
        stringsm_chomp(buf);
        stringsm_rtab(buf);
        stringsm_getfw(&fw, buf, &ind);

        if(strcmp("END", fw)) parser_execins(buf);
        else is_end = true;

        free(buf);
        free(fw);
    }

    return endln;
}
}

static void free_TokenArr(TokenArr* p_arr);
static void interp_ins(TokenArr r_arr);

static void parser_execins(char* p_line)
{
    TokenArr r_arr = INIT_TKN_ARR;

    token_create_arr(&r_arr, (const char*) p_line);
    interp_ins(r_arr);
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

static void interp_func_ins(TokenArr r_arr);

static void interp_ins(TokenArr r_arr)
{
    switch(r_arr.list[0].type)
    {
        case FUNCTION:
            interp_func_ins(r_arr);
            break;
        default:
            perror_disp("this is not yet implemented by the parser", 1);
            break;
    }
}

static void interp_DISPLAY_func(Token* c_list);
static void interp_PRINT_func(Token* c_list);
static void interp_SET_func(Token* c_list);

static void interp_func_ins(TokenArr r_arr)
{
    if(!strcmp(r_arr.list[0].str, "DISPLAY"))
    {
        if(r_arr.ln != 2)
        {
            perror_disp("too many tokens (DISPLAY)", 1);
        }
        interp_DISPLAY_func(r_arr.list);
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
        if(r_arr.ln != 3)
        {
            perror_disp("wrong number of tokens (SET)", 1);
        }
        interp_SET_func(r_arr.list);
    }
}

static void interp_PRINT_func(Token* c_list)
{
    switch(c_list[1].type)
    {
        case STRING:
            {
                char* r_str = NULL;

                stringsm_ext_str_quotes(&r_str, c_list[1].str);
                puts(r_str);

                free(r_str);
            }
            break;
        case STRING_ID:
            printf("\n");
            pstrings_display(c_list[1].str);
            printf("\n");
            break;
        default:
            perror_disp("token cannot be displayed (PRINT)", 0);
            break;
    }
}

static void display_choices(int roomln);

static void interp_DISPLAY_func(Token* c_list)
{
    if(!strcmp(c_list[1].str, "CHOICES"))
    {
        int roomln = -1;
        char* croomid = calloc((P_MAX_BUF_SIZE - 1), sizeof(char));

        pvars_getstdvars("currentroom", &croomid);
        find_roomline(croomid, &roomln);
        display_choices(roomln);
        free(croomid);
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
void parser_splitline(char** type, char** arg, char* ins)
{
    int i = 0;
    int len = 0;
    char* argtocpy = calloc(P_MAX_BUF_SIZE, sizeof(char));
    
    stringsm_chomp(ins);
    len = strlen(ins);
    ins[len] = '\0';
    stringsm_getfw(type, ins, &i);
    if(len != (int)strlen(*type))
    {
        int findex = 0;

        for(int index = i; index < len; index++)
        {
            argtocpy[findex] = ins[index];
            findex++;
        }
        strcpy(*arg, argtocpy);
    } else
    {
        strcpy(*arg, "ARGNULL");
    }

    free(argtocpy);
}


static void display_choicetext(int choiceln, int num);

static void display_choices(int roomln)
{
    int choicesln = 0;
    bool choicesremain = true;
    bool choicesexist = find_choicesline(&choicesln, roomln);
    
    if(!choicesexist)
    {
        perror_disp("NO_CHOICES_INS", 1);
    }
    for(int i = 0; choicesremain; i++)
    {
        int onechoiceln = 0;
        bool choicesexist = find_onechoiceline((i+1), choicesln, &onechoiceln);
        
        if(!choicesexist)
        {
            if(i == 0)
            {
                perror_disp("NO_CHOICE_FND", 1);
            }
            choicesremain = false;
        } else
        {
            display_choicetext(onechoiceln, (i+1));
        }
    }
}

/*Display the text attributed to a choice ; choiceln must point to the
line number of the beginning of the choice block*/
static void display_choicetext(int choiceln, int num)
{
    bool textfound = false;
    bool inif = false;
    char* buf = calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* roomfile = calloc(P_MAX_BUF_SIZE, sizeof(char));
    FILE* fp = NULL;

    pvars_getstdvars("roomfile", &roomfile);
    fileio_setfileptr(&fp, roomfile);
    fileio_gotoline(&fp, choiceln);

    free(roomfile);

    for(int i = 0; !textfound; i++)
    {
        char* arg = calloc(P_MAX_BUF_SIZE - 1, sizeof(char));
        char* type = calloc(P_MAX_BUF_SIZE - 1, sizeof(char));

        fgets(buf, (P_MAX_BUF_SIZE - 1), fp);
        stringsm_chomp(buf);
        stringsm_rtab(buf);
        parser_splitline(&type, &arg, buf);

        if(!strcmp(type, "TEXT"))
        {
            textfound = true;
            printf("\n%i - ", num);
            pstrings_display(arg);
        }
        else if(!strcmp(type, "END"))
        {
            if(inif)
            {
                inif = false;
            } else
            {
                free(buf);
                free(type);
                free(arg);
                perror_disp("NO_CHOICE_TEXT", 1);

                break;
            }
        } else if(buf[0] == 'I' && buf[1] == 'F')
        {
            inif = true;
        }

        free(type);
        free(arg);
    }

    free(buf);
}
