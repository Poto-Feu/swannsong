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

#include <filesystem>
#include <unordered_map>

#include "player/Player.hpp"
#include "room/RoomClass.hpp"
#include "room/RoomLoopState.hpp"
#include "room/RoomState.hpp"
#include "pstrings.hpp"

struct room_property_rtrn
{
    std::string& str;
    bool& use_flag;
};

struct RoomBlockData {
    std::vector<TokenVec>& block_ins;
    std::string const& block_name;
    bool& use_flag;
};

struct RoomVectorData {
    std::vector<std::string>& room_file_lines;
    unsigned int& i;
};

struct RoomCHOICESData {
    std::vector<Choice>& choices_vec;
    bool& use_flag;
};

class RoomManager
{
    public:

        explicit RoomManager(std::filesystem::path const& room_file_path,
                PStrings&& program_strings);
        //Start the game loop which loads rooms until the end signal is enabled
        void startLoop(std::string const& start_room);

    private:

        std::unordered_map<std::string, Room> m_room_map;
        PStrings&& m_program_strings;
        RoomLoopState m_rls;
        Player m_player;

        Room create_new_room(std::vector<std::string> room_file_lines, unsigned int& i,
                bool& no_error, std::string const& room_name);
        void unfinished_game();
        bool set_room_property(std::string const& room_name, std::string const& prop_name,
                std::string const& prop_arg, room_property_rtrn& return_variables);
        bool set_block(std::string const& room_name, RoomVectorData& vec_data,
                RoomBlockData& block_data);
        bool set_CHOICES(std::string const& room_name, RoomVectorData& vec_data,
                RoomCHOICESData& p_data);
};
