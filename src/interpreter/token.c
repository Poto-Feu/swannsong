/*
    Copyright (C) 2020 Adrien Saad

    This file is part of SwannSong.

    SwannSong is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    SwannSong is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "interpreter/token.h"
#include "perror.h"
#include "pstrings.h"

#define FUNC_LIST_SIZE 5
#define OPER_LIST_SIZE 5

#define TKN_STR_BUF 52
#define TKN_ARR_BUF 15

static const char* func_list[FUNC_LIST_SIZE] = 
{
    "PRINT",
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

static void add_tokens_to_arr(token_arr* r_arr, const char* p_str);

/*Create token_arr with the specified string*/
void token_create_arr(token_arr* r_arr, const char* p_str)
{
    if(r_arr->list != NULL)
    {
        perror_disp("r_arr->list is not NULL", 1);
    }
    add_tokens_to_arr(r_arr, p_str);
}


static bool is_oper(char p_chr);
static void create_temp_arr(token* temp_arr, const char* p_str, uint8_t* tkn_n);
static void create_final_arr(token* temp_arr, token_arr* r_arr, uint8_t tkn_n);

/*Find tokens in a string and add them to a token_arr*/
static void add_tokens_to_arr(token_arr* r_arr, const char* p_str)
{
    uint8_t tkn_n = 0;
    token temp_arr[TKN_ARR_BUF];

    create_temp_arr(temp_arr, p_str, &tkn_n);
    create_final_arr(temp_arr, r_arr, tkn_n);
}

static bool is_func(char* p_tkn)
{
    for(int i = 0; i < FUNC_LIST_SIZE; i++)
    {
        if(!strcmp(func_list[i], p_tkn))
        {
            return true;
        }
    }
    return false;
}

static bool is_oper(char p_chr)
{
    for(int i = 0; i < OPER_LIST_SIZE; i++)
    {
        if(p_chr == oper_list[i])
        {
            return true;
        }
    }
    return false;
}

static void set_one_chr_tkn(token* temp_arr, int p_n, char p_chr);

static void create_temp_arr(token* temp_arr, const char* p_str, uint8_t* tkn_n)
{
    int tkn_letter_ind = 0;
    bool is_zero_used = false;

    for(int i = 0; p_str[i] != '\0'; i++)
    {
        if((*tkn_n)+1 == TKN_ARR_BUF)
        {
            perror_disp("too many tokens in rooms line "
                    "(some tokens will be ignored)", 0);
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
                perror_disp("token size is too long", 1);
            }
            temp_arr[*tkn_n].str[tkn_letter_ind] = p_str[i];
            tkn_letter_ind++;
        }
    }
}

static void create_final_arr(token* temp_arr, token_arr* r_arr, uint8_t tkn_n)
{
    r_arr->list = malloc((tkn_n+1) * sizeof(token));
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

static void set_one_chr_tkn(token* temp_arr, int p_n, char p_chr)
{
    temp_arr[p_n].str = calloc(2, sizeof(char));
    temp_arr[p_n].str[0] = p_chr;
}
