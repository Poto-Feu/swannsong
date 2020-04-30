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

#ifndef TEXTUI_H
#define TEXTUI_H

void textui_init();
void textui_exit();
void textui_update();
void textui_newpage();
void textui_showchr();
void textui_hidechr();
void textui_display(const char* p_str);
void textui_readuserinp(char* p_str, int n);
void textui_waitenter();

#endif