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
#include <stdbool.h>
#include <string.h>
#include "init.h"
#include "vars/pconst.h"
#include "vars/pvars.h"
#include "stringsm.h"
#include "fileio/gameconf.h"
#include "room/room.h"

static void ask_lang();

void init_game()
{
    char* defaultlang = calloc(P_MAX_BUF_SIZE, sizeof(char));
    char* room_name = calloc(P_MAX_BUF_SIZE, sizeof(char));

    gameconf_readfile();
    pvars_getgcvars("defaultlang", &defaultlang);
    pvars_setstdvars("lang", defaultlang);
    pvars_getgcvars("firstroom", &room_name);
    pvars_freegcvar("defaultlang");
    pvars_freegcvar("firstroom");

    ask_lang();
    room_load(room_name);

    free(room_name);
    free(defaultlang);
}

/*Show a prompt asking the user to choose the language*/
static void ask_lang()
{
    char* buf = calloc(P_MAX_USERINPUT_SIZE, sizeof(char));
    bool validinp = false;
    char* langarr[2] = {"en", "fr"};

    while(!validinp)
    {
        printf("\nHint : make a choice by typing the corresponding number.\n");
        printf("\nSelect your language:"
                "\n1. English"
                "\n2. Français"
                "\n\nYour choice: ");
        stringsm_getuseri(&buf);
        if(strlen(buf) == 1)
        {
            int intval = buf[0] - '0';
            int langarrsize = (int)sizeof(langarr);
            int langarr0size = (int)sizeof(langarr[0]);
            
            if(intval > 0 && intval <= (langarrsize / langarr0size))
            {
                char* lang = calloc(3, sizeof(char));

                strcpy(lang, langarr[intval - 1]);
                pvars_setstdvars("lang", lang);
                free(lang);
                validinp = true;
            } else
            {
                printf("Nope. (not a valid input)\n");
            }
        } else
        {
            printf("Nope. (too long)\n");
        }
    }

    free(buf);
}
