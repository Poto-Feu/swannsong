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

#ifndef ROOM_MANAGER_HPP
#define ROOM_MANAGER_HPP

#include <vector>
#include "Choice.hpp"
#include "RoomClass.hpp"

class RoomManager
{
    public:

        enum class bt
        {
            ATLAUNCH,
            CHOICE
        };

        RoomManager();

        void endLoop();
        void reset();

        void addTitle();
        void addDesc();
        void addChoice(Choice const& p_choice);
        void addString(std::string const& p_str);
        void addCutscene(std::string const& p_cs);

        void setBlockType(bt const p_bt);
        void setNextRoom(std::string const& p_id);

        unsigned int getChoicesSize() const;
        unsigned int getChoiceLine(unsigned int ch_n) const;
        bt getBlockType() const;
        std::string getNextRoom() const;

        void displayTitle(Room const& p_room);
        void displayDesc(Room const& p_room);
        void displayChoices();
        void displayStrings();
        void displayCutscenes();

        bool is_endgame() const;
        bool is_title_displayed() const;
        bool is_desc_displayed() const;

    private:

        bool endgame = false;
        bool title_displayed = false;
        bool desc_displayed = false;

        bt block_type = bt::ATLAUNCH;

        std::string next_room;

        std::vector<Choice> choice_list;
        std::vector<std::string> string_list;
        std::vector<std::string> cs_list;
};

#endif
