/*
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

#ifndef FIND_H
#define FIND_H

void find_room(char* id, _Bool* inroom, char* lang);
void find_roomline(char* id, int* ln);
_Bool find_atlaunchline(int* foundln, int ln, _Bool* atlfound);
_Bool find_choicesline(int* foundln, int ln, _Bool* choiceslfound);

#endif
