/*
    Copyright (C) 2021 Adrien Saad

    This file is part of SwannSong Adventure.

    SwannSong Adventure is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SwannSong Adventure is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong Adventure.
    If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef CUTSCENE_HPP
#define CUTSCENE_HPP

#include <string>
#include <vector>

class PStrings;

enum class cs_action_type
{
    STRING,
    PAUSE,
    BLANK
};

struct cs_action
{
    cs_action_type type;
    std::string content;
};

struct Cutscene
{
    std::vector<cs_action> actions_vec;
};
#endif
