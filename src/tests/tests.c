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
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "tests.h"
#include "vars/intvar.h"
#include "vars/gvars.h"
#include "pstrings.h"
#include "interpreter/token.h"

void tests_intvar()
{
    #define second_el_val 2889

    int val = 0;
    uint16_t ind = 0;
    bool indexist = false;
    intvar_arr test_arr = INIT_INTVAR_ARR;
    intvar first_el = INIT_INTVAR("test1", 123);
    intvar second_el = INIT_INTVAR("test2", second_el_val);
    intvar third_el = INIT_INTVAR("test3", 1378699);

    intvar_add_var_to_arr(&test_arr, first_el);
    intvar_add_var_to_arr(&test_arr, second_el);
    intvar_add_var_to_arr(&test_arr, third_el);

    indexist = intvar_search_ind(&ind, "testaaa", &test_arr);
    if(indexist)
    {
        printf("intvar_search_ind returns wrong value\n");
        exit(1);
    }

    indexist = intvar_search_ind(&ind, "test2", &test_arr);
    if(ind != 1)
    {
        printf("intvar_search_ind found wrong index\n");
        exit(1);
    }
    intvar_return_value(&val, ind, &test_arr);
    if(val != second_el_val)
    {
        printf("intvar_search_ind found wrong var value\n");
        exit(1);
    }
}

void tests_pstrings()
{
    char str1[] = "ABCDEFG";
    char str2[] = "test_str";
    bool exist1 = pstrings_check_exist(str1);
    bool exist2 = pstrings_check_exist(str2);

    if(exist1 || !exist2)
    {
        printf("pstrings_check_exist returns wrong value\n");
        exit(1);
    }
}

void tests_gvars()
{
    gvars_set_var("test_el1", 123);
    gvars_set_var("test_el2", 8);
    gvars_set_var("test_el3", 671452);
    int test_var1 = gvars_return_value("test_el2");

    if(test_var1 != 8)
    {
        printf("gvars_return_value returns wrong value\n");
        exit(1);
    }

    gvars_change_val("test_el3", 12);

    if(gvars_return_value("test_el3") != 12)
    {
        printf("gvars_change_val sets wrong value\n");
        exit(1);
    }
}

void tests_token()
{
    TokenArr r_arr = {.ln = 0, .list = NULL};
    TokenArr r_arr2 = {.ln = 0, .list = NULL};
    TokenArr r_arr3 = {.ln = 0, .list = NULL};

    token_create_arr(&r_arr, "SET testvar1 = 12 + 3");
    gvars_set_var("testvar2", 12);
    token_create_arr(&r_arr2, "testvar2 = 14");
    token_create_arr(&r_arr3, "PRINT \'hey\'");

    for(int i = 0; i < r_arr.ln; i++)
    {
        free(r_arr.list[i].str);
    }
    free(r_arr.list);
}

