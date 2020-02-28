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
#include "pvars.h"

struct gcvar 
{
    char* name;
    char* value;
};

char pvars_userlang[3] = "en";
struct gcvar pvars_gameconf[3];


void init_gcvars()
{
    pvars_gameconf[0].name = "langdir";
    pvars_gameconf[1].name = "roomfile";
    pvars_gameconf[2].name = "firstroom";
}
