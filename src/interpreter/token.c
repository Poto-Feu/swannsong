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
#include <string.h>
#include <stdbool.h>
#include "interpreter/token.h"
#include "pstrings.h"

#define FUNC_LIST_SIZE 4
static const char* func_list[FUNC_LIST_SIZE] = 
{
    "PRINT",
    "SET",
    "GO",
    "TEXT"
};

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
