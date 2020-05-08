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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "token.h"
#include "perror.h"
#include "pstrings.h"
#include "vars/gvars.h"

#define FUNC_LIST_SIZE 6
#define OPER_LIST_SIZE 5

#define TKN_STR_BUF 52
#define TKN_ARR_BUF 15

static const char* func_list[FUNC_LIST_SIZE] = 
{
    "PRINT",
    "DISPLAY",
    "SET",
    "GO",
    "TEXT",
    "IF"
};

static const char oper_list[OPER_LIST_SIZE] =
{
    '+',
    '-',
    '*',
    '/',
    '%'
};

static void add_tokens_to_arr(TokenArr* r_arr, const char* p_str);

/*Create TokenArr with the specified string*/
void token_create_arr(TokenArr* r_arr, const char* p_str)
{
    if(r_arr->list != NULL)
    {
        perror_disp("r_arr->list is not NULL", 1);
    }
    add_tokens_to_arr(r_arr, p_str);
}

static bool is_oper(char p_chr);
static void create_temp_arr(Token* temp_arr, const char* p_str, uint8_t* tkn_n);
static void create_final_arr(Token* temp_arr, TokenArr* r_arr, uint8_t tkn_n);
static void set_tokens_type(TokenArr* p_arr);

/*Find Tokens in a string and add them to a TokenArr*/
static void add_tokens_to_arr(TokenArr* r_arr, const char* p_str)
{
    uint8_t tkn_n = 0;
    Token temp_arr[TKN_ARR_BUF];

    create_temp_arr(temp_arr, p_str, &tkn_n);
    create_final_arr(temp_arr, r_arr, tkn_n);
    set_tokens_type(r_arr);
}

static void set_one_chr_tkn(Token* temp_arr, int p_n, char p_chr);

static void create_temp_arr(Token* temp_arr, const char* p_str, uint8_t* tkn_n)
{
    int tkn_letter_ind = 0;
    bool is_zero_used = false;

    for(int i = 0; p_str[i] != '\0'; i++)
    {
        if((*tkn_n)+1 == TKN_ARR_BUF)
        {
            perror_disp("too many Tokens in rooms line "
                    "(some Tokens will be ignored)", 0);
            break;
        }
        if(p_str[i] == ' ')
        {
            if(p_str[i+1] == '\0') break;
            tkn_letter_ind = 0;
        } else if(is_oper(p_str[i]))
        {
            (*tkn_n)++;
            tkn_letter_ind = 0;
            set_one_chr_tkn(temp_arr, *tkn_n, p_str[i]);
            temp_arr[*tkn_n].type = OPERATOR;
        } else if(p_str[i] == '=')
        {
            (*tkn_n)++;
            tkn_letter_ind = 0;
            set_one_chr_tkn(temp_arr, *tkn_n, p_str[i]);
            temp_arr[*tkn_n].type = EQUAL;
        } else if(p_str[i] == '!')
        {
            (*tkn_n)++;
            tkn_letter_ind = 0;

            set_one_chr_tkn(temp_arr, *tkn_n, p_str[i]);
            temp_arr[*tkn_n].type = NOT;
        } else if(p_str[i] == '"' || p_str[i] == '\'')
        {
            int quote_ch = p_str[i];
            bool in_string = true;

            (*tkn_n)++;
            tkn_letter_ind = 0;

            temp_arr[*tkn_n].str = calloc(TKN_STR_BUF, sizeof(char));
            temp_arr[*tkn_n].type = STRING;

            /*Create a STRING token*/
            for(int y = i+1; in_string; ++y)
            {
                if(p_str[y] == '\\' && p_str[y+1] == quote_ch)
                {
                    temp_arr[*tkn_n].str[tkn_letter_ind] = p_str[y+1];
                    ++y;
                } else if(p_str[y] != quote_ch)
                {
                    temp_arr[*tkn_n].str[tkn_letter_ind] = p_str[y];
                    tkn_letter_ind++;
                } else
                {
                    i = y;
                    tkn_letter_ind = 0;
                    in_string = false;
                }
            }
        } else
        {
            if(tkn_letter_ind == 0)
            {
                if(is_zero_used) (*tkn_n)++;
                else is_zero_used = true;

                temp_arr[*tkn_n].str = calloc(TKN_STR_BUF, sizeof(char));
                temp_arr[*tkn_n].type = UNDEFINED;
            } else if(tkn_letter_ind == TKN_STR_BUF - 2)
            {
                perror_disp("Token size is too long", 1);
            }
            temp_arr[*tkn_n].str[tkn_letter_ind] = p_str[i];
            tkn_letter_ind++;
        }
    }
}

static void create_final_arr(Token* temp_arr, TokenArr* r_arr, uint8_t tkn_n)
{
    r_arr->list = malloc((tkn_n+1) * sizeof(Token));
    r_arr->ln = tkn_n + 1;

    for(int i = 0; i <= tkn_n; i++)
    {
        int str_ln = strlen(temp_arr[i].str);
        
        r_arr->list[i].str = malloc((str_ln+1) * sizeof(char));
        strcpy(r_arr->list[i].str, temp_arr[i].str);
        r_arr->list[i].str[str_ln] = '\0';
        r_arr->list[i].type = temp_arr[i].type;

        free(temp_arr[i].str);
    }
}

static bool is_if(char* p_tkn);
static bool is_func(char* p_tkn);
static bool is_variable(char* p_tkn);
static bool is_oper(char p_chr);
static bool is_number(char* p_tkn);
static bool is_string(char* p_tkn);
static bool is_string_id(char* p_tkn);
static bool is_new_var(TokenArr* p_arr, int p_ind);
static bool is_exists(char* p_tkn);

/*Set the appropriate type to each Token in a TokenArr*/
static void set_tokens_type(TokenArr* p_arr)
{
    for(int i = 0; i < p_arr->ln; i++)
    {
        if(p_arr->list[i].type != UNDEFINED) continue;

        if(is_if(p_arr->list[i].str)) p_arr->list[i].type = IF;
        else if(is_func(p_arr->list[i].str)) p_arr->list[i].type = FUNCTION;
        else if(is_number(p_arr->list[i].str)) p_arr->list[i].type = NUMBER;
        else if(is_variable(p_arr->list[i].str))
        {
            p_arr->list[i].type = VARIABLE;
        } else if(is_string(p_arr->list[i].str)) p_arr->list[i].type = STRING;
        else if(is_string_id(p_arr->list[i].str))
        {
            p_arr->list[i].type = STRING_ID;
        } else if(is_new_var(p_arr, i)) p_arr->list[i].type = NEWVAR;
        else if(is_exists(p_arr->list[i].str)) p_arr->list[i].type = EXISTS;
        else 
        {
            p_arr->list[i].type = UNKNOWN;
        }
    }
}

static bool is_if(char* p_tkn)
{
    if(!strcmp("IF", p_tkn)) return true;
    return false;
}
static bool is_func(char* p_tkn)
{
    for(int i = 0; i < FUNC_LIST_SIZE; i++)
    {
        if(!strcmp(func_list[i], p_tkn)) return true;
    }
    return false;
}

static bool is_variable(char* p_tkn)
{
    if(gvars_exist(p_tkn)) return true;
    return false;
}

static bool is_oper(char p_chr)
{
    for(int i = 0; i < OPER_LIST_SIZE; i++)
    {
        if(p_chr == oper_list[i]) return true;
    }
    return false;
}

static bool is_number(char* p_tkn)
{
    for(int i = 0; p_tkn[i] != '\0'; i++)
    {
        if(!isdigit(p_tkn[i])) return false;
    }
    return true;
}

static bool is_string(char* p_tkn)
{
    int str_ln = strlen(p_tkn);
    char symbol = '\0';
    
    switch(p_tkn[0])
    {
        case '\'':
            symbol = '\'';
            break;
        case '\"':
            symbol = '\"';
            break;
        default:
            return false;
            break;
    }

    if(p_tkn[str_ln-1] == symbol) return true;
    return false;
}

static bool is_string_id(char* p_tkn)
{
    if(pstrings_check_exist(p_tkn)) return true;
    return false;
}

static bool is_new_var(TokenArr* p_arr, int p_ind)
{
    if(!strcmp(p_arr->list[0].str, "SET") && p_ind == 1) return true;
    return false;
}

static bool is_exists(char* p_tkn)
{
    if(!strcmp(p_tkn, "EXISTS") || !strcmp(p_tkn, "EXIST")) return true;
    return false;
}

static void set_one_chr_tkn(Token* temp_arr, int p_n, char p_chr)
{
    temp_arr[p_n].str = calloc(2, sizeof(char));
    temp_arr[p_n].str[0] = p_chr;
}
