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

#ifndef ROOM_STATE_HPP
#define ROOM_STATE_HPP

#include <string>
#include <vector>
#include "room/Choice.hpp"
#include "room/RoomClass.hpp"

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
        unsigned int getChoiceLine(unsigned int ch_n) const;
        bt getBlockType() const;

        void addTitle();
        void addDesc();
        void addChoice(Choice const& p_choice);
        void addString(std::string const& p_str);
        void addCutscene(std::string const& p_cs);

        void displayCutscenes();
        void displayAll(Room const& p_room);

        void setBlockType(bt const p_bt);

        bool is_title_displayed() const;
        bool is_desc_displayed() const;

    private:

        bool title_displayed = false;
        bool desc_displayed = false;

        std::vector<Choice> choice_list;
        std::vector<std::string> string_list;
        std::vector<std::string> cs_list;

        bt block_type = bt::ATLAUNCH;

        void displayTitle(Room const& p_room);
        void displayDesc(Room const& p_room);
        void displayChoices();
        void displayStrings();
};

#endif
