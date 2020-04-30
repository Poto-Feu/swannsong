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

#ifndef PCURSES_H
#define PCURSES_H

void pcurses_init();
void pcurses_end();
void pcurses_refresh();
void pcurses_clear();
void pcurses_display_text(const char* p_str);
void pcurses_echo();
void pcurses_noecho();
void pcurses_getnstr(char* p_str, int n);
int pcurses_getch();

#endif
