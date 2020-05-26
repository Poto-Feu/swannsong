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

#include "token.hpp"
#include "perror.h"
#include "pstrings.h"
#include "vars/gvars.hpp"

static const std::vector<std::string> func_list = 
{
    "PRINT",
    "DISPLAY",
    "SET",
    "GO",
    "TEXT",
    "IF",
    "CUTSCENE",
    "UNFINISHED"
};

static const std::vector<char> oper_list =
{
    '+',
    '-',
    '*',
    '/',
    '%'
};

static bool is_oper(char p_chr);
static TokenVec create_token_vector(std::string p_str);
static void set_tokens_type(TokenVec& p_arr);

/*Find Tokens in a string and add them to a TokenVec*/
static TokenVec add_tokens_to_arr(std::string p_str)
{
    TokenVec r_vec = create_token_vector(p_str);
    set_tokens_type(r_vec);

    return r_vec;
}

static TokenVec create_token_vector(std::string p_str)
{
    int str_size = p_str.size();
    TokenVec rtrn_val;

    for(int i = 0; i < str_size; ++i)
    {
        if(p_str[i] == ' ')
        {
            if(i+1 == str_size) break;
        } else if(is_oper(p_str[i]))
        {
            Token new_tkn;

            new_tkn.str += p_str[i];
            new_tkn.type = token_type::OPERATOR;
            rtrn_val.push_back(new_tkn);
        } else if(p_str[i] == '=')
        {
            Token new_tkn;

            new_tkn.str += p_str[i];
            new_tkn.type = token_type::EQUAL;
            rtrn_val.push_back(new_tkn);
        } else if(p_str[i] == '!')
        {
            Token new_tkn;

            new_tkn.str += p_str[i];
            new_tkn.type = token_type::NOT;
            rtrn_val.push_back(new_tkn);
        } else if(p_str[i] == '"' || p_str[i] == '\'')
        {
            int quote_ch = p_str[i];
            bool in_string = true;
            Token new_tkn;

            new_tkn.type = token_type::STRING;

            /*Create a STRING token*/
            for(int y = i+1; in_string; ++y)
            {
                if(p_str[y] == '\\' && p_str[y+1] == quote_ch)
                {
                    new_tkn.str += p_str[y+1];
                    ++y;
                } else if(p_str[y] != quote_ch) new_tkn.str += p_str[y];
                else
                {
                    i = y;
                    in_string = false;
                }
            }
            rtrn_val.push_back(new_tkn);
        } else
        {
            Token new_tkn;

            for(int y = i; i < str_size; ++y)
            {
                new_tkn.str += p_str[y];

                if(y + 1 < str_size)
                {
                    if(p_str[y + 1] == ' ' || p_str[y + 1] == '\t')
                    {
                        rtrn_val.push_back(new_tkn);
                        i = y;
                        break;
                    }
                } else
                {
                    rtrn_val.push_back(new_tkn);
                    i = y;
                    break;
                }
            }
        }
    }
    
    return rtrn_val;
}

static bool is_if(std::string p_tkn);
static bool is_func(std::string p_tkn);
static bool is_variable(std::string p_tkn);
static bool is_oper(char p_chr);
static bool is_number(std::string p_tkn);
static bool is_string(std::string p_tkn);
static bool is_string_id(std::string p_tkn);
static bool is_new_var(TokenVec& p_arr, int p_ind);
static bool is_exists(std::string p_tkn);

/*Set the appropriate type to each Token in a TokenVec*/
static void set_tokens_type(TokenVec& p_vec)
{
    int i = 0;
    for(auto& it : p_vec)
    {
        if(it.type == token_type::UNDEFINED)
        {
            if(is_if(it.str)) it.type = token_type::IF;
            else if(is_func(it.str)) it.type = token_type::FUNCTION;
            else if(is_number(it.str)) it.type = token_type::NUMBER;
            else if(is_variable(it.str))
            {
                it.type = token_type::VARIABLE;
            } else if(is_string(it.str)) it.type = token_type::STRING;
            else if(is_string_id(it.str))
            {
                it.type = token_type::STRING_ID;
            } else if(is_new_var(p_vec, i)) it.type = token_type::NEWVAR;
            else if(is_exists(it.str)) it.type = token_type::EXISTS;
            else it.type = token_type::UNKNOWN;
        }
        ++i;
    }
}

static bool is_if(std::string p_tkn)
{
    if(p_tkn == "IF") return true;
    return false;
}

static bool is_func(std::string p_tkn)
{
    for(auto const& it : func_list)
    {
        if(p_tkn == it) return true;
    }
    return false;
}

static bool is_variable(std::string p_tkn)
{
    if(gvars::exist(p_tkn)) return true;
    return false;
}

static bool is_oper(char p_chr)
{
    for(auto const& it : oper_list)
    {
        if(p_chr == it) return true;
    }
    return false;
}

static bool is_number(std::string p_tkn)
{
    for(int i = 0; p_tkn[i] != '\0'; i++)
    {
        if(!isdigit(p_tkn[i])) return false;
    }
    return true;
}

static bool is_string(std::string p_tkn)
{
    int str_ln = p_tkn.size();
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

static bool is_string_id(std::string p_tkn)
{
    if(pstrings::check_exist(p_tkn)) return true;
    return false;
}

static bool is_new_var(TokenVec& p_vec, int p_ind)
{
    if(p_vec[0].str == "SET" && p_ind == 1) return true;
    return false;
}

static bool is_exists(std::string const p_tkn)
{
    if(p_tkn == "EXISTS" || p_tkn ==  "EXIST") return true;
    return false;
}

namespace token
{
    /*Create TokenVec with the specified string*/
    TokenVec create_arr(std::string p_str)
    {
        TokenVec rtrn_vec = add_tokens_to_arr(p_str);

        return rtrn_vec;
    }
}
