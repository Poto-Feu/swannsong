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
    along with SwannSong Adventure.
    If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "vars/gvars.hpp"

class PStrings;
class Player;

enum class token_type
{
    FUNCTION,
    IF,
    ELSE,
    END,
    VARIABLE,
    OPERATOR,
    EQUAL,
    NOT,
    HAS,
    STRING,
    NUMBER,
    STRING_ID,
    ITEM,
    NEWVAR,
    TEXT,
    EXISTS,
    UNKNOWN,
    UNDEFINED
};

enum class token_spec_type
{
    NONE,
    PRINT,
    DISPLAY,
    SET,
    GO,
    CUTSCENE,
    GET,
    USE,
    GAMEOVER,
    EXIT,
};

struct Token
{
    std::string str = "";
    token_type type = token_type::UNDEFINED;
    token_spec_type spec_type = token_spec_type::NONE;
};

typedef std::vector<Token> TokenVec;

namespace token
{
    TokenVec create_arr(std::string const& p_str, PStrings const& program_strings);
    void set_runtime_tokens(TokenVec& p_vec, gvarVector const& p_gvars);
}
#endif
