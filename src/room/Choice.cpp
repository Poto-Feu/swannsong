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

#include <algorithm>

#include "room/Choice.hpp"
#include "game_error.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "stringsm.h"

Choice::Choice() { }
Choice::Choice(unsigned int choice_n, std::vector<TokenVec>&& instructions) : m_id(choice_n),
    m_instructions(instructions)
{
    auto TEXT_prop = std::find_if(m_instructions.cbegin(), m_instructions.cend(),
            [&] (TokenVec const& tkns) {
            return tkns[0].type == token_type::TEXT && tkns.size() == 2;
            });

    if(TEXT_prop != m_instructions.cend()) {
        if((*TEXT_prop)[1].type == token_type::STRING) {
            m_text = stringsm::ext_str_quotes((*TEXT_prop)[1].str);
        } else if((*TEXT_prop)[1].type == token_type::STRING_ID) {
            m_text = pstrings::fetch((*TEXT_prop)[1].str);
        } else game_error::fatal_error("TEXT token not followed by STRING or STRING_ID token");
    }
}

unsigned int Choice::getId() const
{
    return m_id;
}

void Choice::display() const
{
    int str_line = display_server::get_last_line() + 1;
    std::string disp_value = m_text;

    disp_value.insert(0, ". ");
    disp_value.insert(0, std::to_string(m_id));

    if(str_line == display_server::LAST_LINE_ERR + 1) str_line = pcurses::title_y + 4;
    pcurses::display_pos_string(disp_value, pcurses::choice_space, str_line);
}

std::vector<TokenVec> const& Choice::getInstructions() const
{
    return m_instructions;
}
