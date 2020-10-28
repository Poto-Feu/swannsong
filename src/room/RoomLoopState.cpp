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

#include "room/RoomLoopState.hpp"

void RoomLoopState::endLoop()
{
    m_endgame = true;
}

void RoomLoopState::setNextRoom(std::string const& p_id)
{
    m_next_room = p_id;
}

void RoomLoopState::setUnfinished()
{
    m_unfinished = true;
}

void RoomLoopState::setGameOver()
{
    m_gameover = true;
}

void RoomLoopState::resetGameOver()
{
    m_gameover = false;
}

std::string RoomLoopState::getNextRoom() const
{
    return m_next_room;
}

bool RoomLoopState::is_endgame() const
{
    return m_endgame;
}

bool RoomLoopState::is_unfinished() const
{
    return m_unfinished;
}

bool RoomLoopState::is_game_over() const
{
    return m_gameover;
}
