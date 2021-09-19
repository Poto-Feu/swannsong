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

#ifndef USERIO_HPP
#define USERIO_HPP

#define WIN_ENTER_KEY 13

#include <string>
#include <unordered_map>

struct Player;
struct game_state_s;
class CutscenesContainer;
class PStrings;
class Room;
class RoomDisplay;
class RoomLoopState;

namespace userio
{
    void waitenter();
    std::string gettextinput(int max_n);
    bool interpret_user_input(PStrings const& pstrings,
            std::unordered_map<std::string, Room> const& room_map,
            CutscenesContainer const& cs_container, Room const& room,
            Player& player, RoomDisplay const& room_display,
            RoomLoopState& rls, game_state_s& game_state, std::string& input,
            bool& has_wrong_input);
}
#endif
