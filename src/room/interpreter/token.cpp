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

#include <array>
#include <algorithm>

#include "room/interpreter/token.hpp"
#include "stringsm.hpp"

//Set the appropriate type to each Token in a TokenVec
namespace token
{
    static bool is_oper(char p_chr)
    {
        static const std::array<char, 2> oper_list =
        {
            '+',
            '-'
        };

        return std::find_if(oper_list.cbegin(), oper_list.cend(), [=](char cch) {
                return p_chr == cch;
                }) != oper_list.cend();
    }

    void add_one_char(TokenVec& tkn_vec, char p_chr, token_type const& p_type)
    {
        Token new_tkn;

        new_tkn.str += p_chr;
        new_tkn.type = p_type;
        tkn_vec.push_back(new_tkn);
    }

    static void add_unknown_token(TokenVec& p_vec, std::string const& p_str, size_t &i)
    {
        Token new_tkn;
        size_t str_size = p_str.size();

        for(size_t y = i; i < str_size; ++y) {
            new_tkn.str += p_str[y];

            if(y + 1 < str_size) {
                if(p_str[y + 1] == ' ' || p_str[y + 1] == '\t') {
                    p_vec.push_back(new_tkn);
                    i = y;
                    break;
                }
            } else {
                p_vec.push_back(new_tkn);
                i = y;
                break;
            }
        }
    }

    auto create_token_vector(std::string const& p_str)
    {
        size_t str_size = p_str.size();
        TokenVec tkn_vec;

        for(size_t i = 0; i < str_size; ++i) {
            if(p_str[i] == ' ') {
                if(i+1 == str_size) break;
            } else if(is_oper(p_str[i])) add_one_char(tkn_vec, p_str[i], token_type::OPERATOR);
            else if(p_str[i] == '=') add_one_char(tkn_vec, p_str[i], token_type::EQUAL);
            else if(p_str[i] == '!') add_one_char(tkn_vec, p_str[i], token_type::NOT);
            else if(p_str[i] == '"' || p_str[i] == '\'') {
                int quote_ch = p_str[i];
                bool in_string = true;
                Token new_tkn;

                new_tkn.type = token_type::STRING;

                //Create a STRING token
                for(size_t y = i+1; in_string; ++y) {
                    if(p_str[y] == '\\' && p_str[y+1] == quote_ch) {
                        new_tkn.str += p_str[y+1];
                        ++y;
                    } else if(p_str[y] != quote_ch) new_tkn.str += p_str[y];
                    else {
                        i = y;
                        in_string = false;
                    }
                }
                tkn_vec.push_back(new_tkn);
            } else add_unknown_token(tkn_vec, p_str, i);
        }
        return tkn_vec;
    }

    static bool is_func(Token& p_tkn)
    {
        struct func_list_item
        {
            std::string str;
            token_spec_type spec_type;
        };

        //An enum is used for faster condition checking in the parser
        const std::array<func_list_item, 10> func_list = {
            {
                { "PRINT", token_spec_type::PRINT },
                { "DISPLAY", token_spec_type::DISPLAY },
                { "SET", token_spec_type::SET },
                { "GO", token_spec_type::GO },
                { "CUTSCENE", token_spec_type::CUTSCENE },
                { "GET", token_spec_type::GET },
                { "USE", token_spec_type::USE },
                { "GAMEOVER", token_spec_type::GAMEOVER },
                { "EXIT", token_spec_type::EXIT },
            }
        };

        auto found_it = std::find_if(func_list.cbegin(), func_list.cend(),
                [&p_tkn](func_list_item const& cfunc) {
                return cfunc.str == p_tkn.str;
                });

        if(found_it != func_list.cend()) {
            p_tkn.type = token_type::FUNCTION;
            p_tkn.spec_type = found_it->spec_type;
            return true;
        } else return false;
    }

    static bool is_number(std::string& p_tkn)
    {
        //Make true and false aliases for integers values
        if(p_tkn == "true") {
            p_tkn = "1";
            return true;
        } else if(p_tkn == "false") {
            p_tkn = "0";
            return true;
        }
        return stringsm::is_number(p_tkn);
    }

    static bool is_string(std::string const& p_tkn)
    {
        size_t str_ln = p_tkn.size();
        char symbol = '\0';

        switch(p_tkn[0]) {
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
        else return false;
    }

    static bool is_item(TokenVec const& p_vec, int p_ind)
    {
        if(p_vec[0].str == "GET" && (p_ind == 1 || p_ind == 2)) return true;
        else return false;
    }

    static void set_tokens_type(TokenVec& p_vec,
            pstrings::ps_data_ptr const& pstrings_data)
    {
        int i = 0;

        for(auto& it : p_vec) {
            if(it.type == token_type::UNDEFINED) {
                if(it.str == "IF") it.type = token_type::IF;
                else if(it.str == "ELSE") it.type = token_type::ELSE;
                else if(it.str == "!" || it.str == "NOT") it.type = token_type::NOT;
                else if(it.str == "HAS") it.type = token_type::HAS;
                else if(it.str == "END") it.type = token_type::END;
                else if(is_func(it)) it.type = token_type::FUNCTION;
                else if(is_number(it.str)) it.type = token_type::NUMBER;
                else if(is_string(it.str)) it.type = token_type::STRING;
                else if(pstrings::check_if_exists(pstrings_data, it.str)) {
                    it.type = token_type::STRING_ID;
                }
                else if(it.str == "TEXT") it.type = token_type::TEXT;
                else if(p_vec[0].str == "SET" && i == 1) it.type = token_type::NEWVAR;
                else if(it.str == "EXISTS" || it.str == "EXIST") it.type = token_type::EXISTS;
                else if(is_item(p_vec, i)) it.type = token_type::ITEM;
                else it.type = token_type::UNKNOWN;
            }
            ++i;
        }
    }

    //Create TokenVec with the specified string
    TokenVec create_arr(std::string const& p_str,
            pstrings::ps_data_ptr const& pstrings_data)
    {
        TokenVec r_vec = create_token_vector(p_str);

        set_tokens_type(r_vec, pstrings_data);
        return r_vec;
    }

    void set_runtime_tokens(TokenVec& p_vec, gvarVector const& p_gvars)
    {
        for(auto& it : p_vec) {
            if(it.type == token_type::UNDEFINED || it.type == token_type::UNKNOWN) {
                if(gvars::exist(p_gvars, it.str)) it.type = token_type::VARIABLE;
            }
        }
    }
}
