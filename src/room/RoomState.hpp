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

#include "room/Choice.hpp"
#include "room/RoomClass.hpp"
#include "pstrings.hpp"

class RoomState
{
    public:

        RoomState();

        enum class bt
        {
            ATLAUNCH,
            CHOICE
        };

        unsigned int getChoicesSize() const;
        bt getBlockType() const;

        void addTitle();
        void addDesc();
        void addAllChoices();
        void addString(std::string const& p_str);
        void addCutscene(std::string const& p_cs);

        void displayCutscenes(PStrings const& program_strings);
        void displayTitle(Room const& p_room) const;
        void displayDesc(Room const& p_room) const;
        void displayAll(Room const& p_room, PStrings const& program_strings);

        void setBlockType(bt const p_bt);

        bool is_title_displayed() const;
        bool is_desc_displayed() const;

    private:

        bool title_displayed = false;
        bool desc_displayed = false;
        bool m_all_choices_displayed = false;

        std::vector<std::string> string_list;
        std::vector<std::string> cs_list;

        bt block_type = bt::ATLAUNCH;

        void displayChoices(Room const& p_room);
        void displayStrings();
};
