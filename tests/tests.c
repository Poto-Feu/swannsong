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
#include <stdint.h>
#include <stdbool.h>
#include "tests.h"
#include "vars/intvar.h"

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

void tests_runall()
{
    tests_intvar();
}
