/*
    Copyright (C) 2021 Adrien Saad

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
    along with SwannSong Adventure.  If not, see
    <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <string>

class RoomLoopState
{
    public:

        //Trigger a game over
        void setGameOver();
        void resetGameOver();
        //Set the room in which the player will spawn at the next loop iteration
        void setNextRoom(std::string const& p_id);

        std::string getNextRoom() const;

        bool is_game_over() const;

    private:

        bool m_gameover = false;

        std::string m_next_room;
};
