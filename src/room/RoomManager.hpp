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

#include <string>

class RoomManager
{
    public:

        RoomManager();

        void endLoop();

        void setUnfinished();
        void setNextRoom(std::string const& p_id);

        std::string getNextRoom() const;

        bool is_endgame() const;
        bool is_unfinished() const;

    private:

        bool endgame = false;
        bool unfinished = false;

        std::string next_room;
};

#endif
