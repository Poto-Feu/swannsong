/*
    Copyright (C) 2020 Adrien Saad

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
    along with SwannSong Adventure.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <vector>

#include "pstrings.hpp"
#include "cutscenes.hpp"

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

class Cutscene
{
    public:

        Cutscene();

        std::string name;
        std::vector<cs_action> actions_vec;

        void display(PStrings const& program_strings) const;

    private:

        void execute_all_actions(PStrings const& program_strings) const;
};
