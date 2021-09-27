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
#include "stringsm.hpp"

Choice::Choice() { }
Choice::Choice(unsigned int choice_n, std::vector<TokenVec>&& instructions,
        pstrings::ps_data_ptr const& pstrings_data) : m_id(choice_n),
    m_instructions(instructions)
{
    auto TEXT_prop = std::find_if(m_instructions.cbegin(), m_instructions.cend(),
            [&] (TokenVec const& tkns) {
            return tkns[0].type == token_type::TEXT && tkns.size() == 2;
            });

    if(TEXT_prop != m_instructions.cend()) {
        if((*TEXT_prop)[1].type == token_type::STRING) {
            m_text = stringsm::ext_str_quotes((*TEXT_prop)[1].str);
        } else {
            m_text = pstrings::fetch_string(pstrings_data,
                    (*TEXT_prop)[1].str);
        }
        if((*TEXT_prop)[1].type != token_type::STRING_ID) {
            game_error::emit_warning(
                    "TEXT token not followed by STRING or STRING_ID token ("
                    + (*TEXT_prop)[1].str + ")");
        }
    }
}

unsigned int Choice::getId() const
{
    return m_id;
}

std::string Choice::getText() const
{
    return m_text;
}

std::vector<TokenVec> const& Choice::getInstructions() const
{
    return m_instructions;
}
