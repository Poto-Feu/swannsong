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

#ifndef ROOM_HPP
#define ROOM_HPP

#include <string>
#include <vector>
#include "cutscenes.hpp"

void room_load(std::string id);

class Choice
{
    public:

        Choice(int ch_n, int ch_ln);

        void display();

    private:

        int choice_n;
        int choice_line;
};

class Room 
{
    public:

        Room(std::string room_name);

        void getName(char* r_name) const;
        std::string getName() const;

        bool isRoomLineSet() const;
        bool isChoicesLineSet() const;

        int getRoomLine() const;
        int getChoicesLine() const;

        void setRoomLine(int rln);
        void setChoicesLine(int chln);

    private:

        std::string name;

        int room_line = 0;
        int choices_line = 0;
};

class RoomManager
{
    public:

        RoomManager();

        void addTitle();
        void addDesc();
        void addChoice(Choice p_choice);
        void addString(std::string p_str);
        void addCutscene(std::string const p_cs);

        void displayTitle(Room p_room);
        void displayDesc(Room p_room);
        void displayChoices();
        void displayStrings();
        void displayCutscenes();

        bool is_title_displayed();
        bool is_desc_displayed();

    private:

        bool title_displayed = false;
        bool desc_displayed = false;

        std::vector<Choice> choice_list;
        std::vector<std::string> string_list;
        std::vector<std::string> cs_list;
};

#endif
