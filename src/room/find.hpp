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


#ifndef FIND_H
#define FIND_H

#include <string>

bool find_atlaunchline(int* foundln, int ln);
bool find_choicesline(int* foundln, int ln);
bool find_onechoiceline(int num, int startln, int* ln);
void find_room(char* id, bool* inroom, char* lang);
int find_roomline(const char* id);
bool find_room_property(std::string& value, const char* prop, int roomln);

#endif
