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

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <vector>

enum class token_type
{
    FUNCTION,
    IF,
    VARIABLE,
    OPERATOR,
    EQUAL,
    NOT,
    STRING,
    NUMBER,
    STRING_ID,
    ITEM,
    NEWVAR,
    EXISTS,
    UNKNOWN,
    UNDEFINED
};

struct Token
{
    std::string str = "";
    token_type type = token_type::UNDEFINED;
};

typedef std::vector<Token> TokenVec;

namespace token
{
    TokenVec create_arr(std::string const& p_str);
}
#endif
