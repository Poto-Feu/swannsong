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
#include <stdbool.h>
#include <string.h>
#include <curses.h>
#include "init.h"
#include "vars/pvars.h"
#include "fileio/gameconf.h"
#include "room/room_io.h"
#include "room/room.h"
#include "userio.h"
#include "pstrings.h"

static void init_curses();
static void init_pvars(char** room_name);
static void ask_lang();

void init_game()
{
    char* room_name = NULL; 

    init_curses();

    init_pvars(&room_name);
    roomio_copy_file_to_vec();

    ask_lang();
    room_load(room_name);

    free(room_name);
}

static void init_curses()
{
    initscr();
    raw();
    noecho();
}

static void init_pvars(char** room_name)
{
    char* defaultlang = NULL;
    char* roomfile = NULL;

    gameconf_readfile();
    pvars_getgcvars("defaultlang", &defaultlang);
    pvars_getgcvars("firstroom", room_name);
    pvars_getgcvars("roomfile", &roomfile);
    pvars_setstdvars("lang", defaultlang);
    pvars_setstdvars("roomfile", roomfile);
    pvars_freegcvar("defaultlang");
    pvars_freegcvar("firstroom");
    pvars_freegcvar("roomfile");

    free(defaultlang);
    free(roomfile);
}

/*Show a prompt asking the user to choose the language*/
static void ask_lang()
{
    char* buf = NULL;
    char* langarr[2] = {"en", "fr"};
    bool validinp = false;

    clear();
    printw("Hint : make a choice by typing the corresponding number.\n");
    printw("\nSelect your language:"
            "\n1. English"
            "\n2. Français\n");

    while(!validinp)
    {
        printw("\nYour choice: ");
        refresh();

        userio_gettextinput(&buf, 2);

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
                validinp = true;
                pstrings_copy_file_to_vec();

                free(lang);
            } else
            {
                printw("Nope. (not a valid input)\n");
                refresh();
            }
        } else
        {
            printw("Nope. (too long)\n");
            refresh();
        }
        free(buf);
    }
}
