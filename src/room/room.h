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

#ifndef ROOM_H
#define ROOM_H

#ifdef __cplusplus
extern "C" {
#endif

void room_load(char* id);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <string>
#include <vector>

enum display_type
{
    TITLE,
    DESC,
    CHOICE
};

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

        bool isRoomLineSet() const;
        bool isChoicesLineSet() const;

        int getRoomLine() const;
        int getChoicesLine() const;

        void setRoomLine(int rln);
        void setChoicesLine(int chln);

        void displayList();

        void addDisplayTitle();
        void addDisplayDesc();
        void addDisplayChoice(int ch_ln);

    private:

        std::string name;

        int room_line = 0;
        int choices_line = 0;

        std::vector<display_type> display_order;
        std::vector<Choice> displayed_choices;

        void displayTitle(bool& title_shown);
};
#endif

#endif
