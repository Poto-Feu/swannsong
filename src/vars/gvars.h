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

#ifndef GVARS_H
#define GVARS_H

#ifdef __cplusplus
extern "C" {
#endif

void gvars_set_var(char* p_name, int p_val);
int gvars_return_value(char* p_name);
void gvars_change_val(char* p_name, int p_val);
bool gvars_exist(char* p_name);

#ifdef __cplusplus
}
#endif

#endif
