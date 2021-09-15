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

#ifndef ROOM_STATE_HPP
#define ROOM_STATE_HPP

#include "CutscenesContainer.hpp"
#include "room/Choice.hpp"

class PStrings;

class RoomState
{
    public:

        enum class bt
        {
            ATLAUNCH,
            CHOICE
        };

        unsigned int getChoicesSize() const;
        bt getBlockType() const;

        void addTitle();
        void addDesc();
        void addAllChoices();
        //Add a choice to be displayed on the menu screen
        void addChoice(unsigned int choice_n);
        void addString(std::string const& p_str);
        void addCutscene(std::string const& p_cs);

        void displayCutscenes(PStrings const& pstrings,
                CutscenesContainer const& cs_container);
        std::string displayRoomScreen(PStrings const& pstrings,
                std::string const* room_name, std::string const* room_title,
                std::string const* room_desc,
                std::vector<Choice> room_choices,
                const std::string *error_msg = nullptr) const;
        std::string displayAll(PStrings const& pstrings,
                CutscenesContainer const& cs_container,
                std::string const* room_name, std::string const* room_title,
                std::string const* room_desc,
                std::vector<Choice> room_choices, bool same_room);

        /* Return the Choice id corresponding to the specified index of the
         * displayed choices vector */
        unsigned int getCorrespondantChoiceId(unsigned int choice_n) const;

        void setBlockType(bt const p_bt);

        bool is_title_displayed() const;
        bool is_desc_displayed() const;
        bool is_all_choices_displayed() const;

    private:

        bool m_title_displayed = false;
        bool m_desc_displayed = false;
        bool m_all_choices_displayed = false;

        std::vector<unsigned int> m_choices_list;
        std::vector<std::string> m_string_list;
        std::vector<std::string> m_cutscenes_list;

        bt block_type = bt::ATLAUNCH;
};
#endif
