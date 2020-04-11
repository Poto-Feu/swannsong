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

#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>

typedef enum
{
    FUNCTION,
    VARIABLE,
    OPERATOR,
    EQUAL,
    STRING,
    NUMBER,
    STRING_ID,
    NEWVAR,
    UNKNOWN,
    UNDEFINED
} token_type;

typedef struct
{
    char* str;
    token_type type;
} token;

typedef struct
{
    uint8_t ln;
    token* list;
} token_arr;


void token_create_arr(token_arr* r_arr, const char* p_str);

#endif
