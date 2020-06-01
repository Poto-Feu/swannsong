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

#include "RoomClass.hpp"

Room::Room() { }
Room::Room(std::string const& room_name) : name(room_name) { }

std::string Room::getName() const
{
    return name;
}

bool Room::isChoicesLineSet() const
{
    if(choices_line != 0) return true;
    else return false;
}

int Room::getRoomLine() const 
{
    return room_line;
}

int Room::getChoicesLine() const 
{
    return choices_line;
}

void Room::setRoomLine(int rln) 
{
    room_line = rln;
}

void Room::setChoicesLine(int chln)
{
    choices_line = chln;
}
