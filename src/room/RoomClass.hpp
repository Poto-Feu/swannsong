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

#include <optional>
#include <unordered_map>

#include "room/interpreter/token.hpp"
#include "room/RoomLoopState.hpp"
#include "room/Choice.hpp"
#include "player/Player.hpp"
#include "pstrings.hpp"

class Room 
{
    public:

        Room();
        explicit Room(std::string const& room_name);
        explicit Room(std::string const& room_name, std::string const& room_title);
        explicit Room(std::string const& room_name, std::string const& room_title,
                std::string const& room_desc);

        std::string const& getName() const;
        std::string const& getTitle() const;
        std::string const& getDesc() const;

        std::vector<TokenVec> const& getATLAUNCHIns() const;
        //Use a const reference to prevent excessive copying
        std::vector<Choice> const& getChoicesVec() const;

        //Get the number of choices defined in the room
        unsigned int getChoicesSize() const;
        const Choice *getChoice(unsigned int choice_n) const;

        void setDesc(std::string const& room_desc);
        void setATLAUNCH_ins(std::vector<TokenVec>&& atlaunch_ins);
        void setChoices(std::vector<Choice>&& choices_vec);

        //Check if the choice with the specified number is defined
        bool isChoicePresent(unsigned int choice_n) const;

        void displayAllChoices() const;

        bool load(RoomLoopState& p_rls, Player& p_player,
                std::unordered_map<std::string, Room> const& room_map,
                PStrings const& program_strings);

    private:

        std::string m_name;
        std::string m_title;
        std::string m_desc;

        std::vector<TokenVec> m_ATLAUNCH_ins;
        std::vector<Choice> m_Choices_vec;

        void displayTitle(Room const& p_room);
        void displayDesc(Room const& p_room);
};
