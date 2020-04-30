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
#include <string.h>
#include <stdbool.h>
#include "perror.h"
#include "textui/textui.h"
#include "exitgame.h"

/*Display an error message with the specified string*/
void perror_disp(const char* id, bool fatal)
{
    const char* chr_err = "ERROR : ";

    int chr_err_ln = strlen(chr_err);
    int id_ln = strlen(id);
    int full_chr_ln = chr_err_ln + id_ln;
    char* full_chr = calloc(chr_err_ln + id_ln + 1, sizeof(char));

    strcpy(full_chr, chr_err);

    for(int i = 0; i < id_ln; i++)
    {
        full_chr[i + chr_err_ln] = id[i];
    }

    full_chr[full_chr_ln] = '\n';

    textui_newpage();
    textui_display(full_chr);
    textui_update();

    if (fatal) exitgame(1);
}
