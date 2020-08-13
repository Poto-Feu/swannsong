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

#ifndef ROOMCLASS_HPP
#define ROOMCLASS_HPP

#include <string>
#include "room/RoomManager.hpp"

class Room 
{
    public:

        Room();
        explicit Room(std::string const& room_name);

        void getName(char* r_name) const;
        std::string getName() const;

        bool isChoicesLineSet() const;

        int getRoomLine() const;
        int getChoicesLine() const;

        void setRoomLine(int rln);
        void setChoicesLine(int chln);

        bool load(RoomManager& p_rmm);

    private:

        std::string name;

        int room_line = 0;
        int choices_line = 0;
};

#endif
