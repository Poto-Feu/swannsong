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

#include <algorithm>
#include <fstream>

#include "CutscenesContainer.hpp"
#include "fileio/fileio.h"
#include "dialogbox.hpp"
#include "files_path.hpp"
#include "game_error.hpp"
#include "pcurses.hpp"
#include "rendering.hpp"
#include "stringsm.h"
#include "userio.h"

CutscenesContainer::CutscenesContainer()
{
}

CutscenesContainer::CutscenesContainer(std::string const& csfile,
        std::filesystem::path const& data_path, PStrings const& program_strings)
{
    std::string buf;
    std::ifstream file_stream(data_path.string() + csfile);

    while(fileio::getfileln(buf, file_stream)) {
        Cutscene curr_cs;

        stringsm::rtab(buf);

        if(buf == "" || buf[0] == '#') continue;

        std::string cs_name = stringsm::getfw(buf);

        if(check_exist(cs_name)) {
            game_error::fatal_error("duplicate cutscenes");
            return;
        }

        while(fileio::getfileln(buf, file_stream)) {
            std::string fw;
            cs_action curr_action;

            stringsm::rtab(buf);
            fw = stringsm::getfw(buf);

            if(fw == "PAUSE") curr_action.type = cs_action_type::PAUSE;
            else if(fw == "BLANK") curr_action.type = cs_action_type::BLANK;
            else if(stringsm::is_str(buf)) {
                curr_action.type = cs_action_type::STRING;
                curr_action.content = stringsm::ext_str_quotes(buf);
            } else if(fw == "END") break;
            else {
                curr_action.type = cs_action_type::STRING;
                curr_action.content = program_strings.fetch(fw);
            }
            curr_cs.actions_vec.push_back(curr_action);
        }
        m_map[cs_name] = curr_cs;
    }
}

//Display a cutscene
void CutscenesContainer::display(PStrings const& pstrings,
        std::string const& name) const
{
    auto it = return_cs_it(name);
    display_server::clear_screen();

    if(it != m_map.cend()) {
        rendering::display_cutscene(pstrings, it->second);
    } else {
        const std::string missingCutscene_text = "missingCutscene";

        game_error::emit_warning("\"" + name + "\" cutscene does not exists");
        dialogbox::display(&missingCutscene_text, NULL, pstrings);
    }
}

std::unordered_map<std::string, Cutscene>::const_iterator CutscenesContainer::return_cs_it(
        std::string const& cutscene_name) const
{
    return m_map.find(cutscene_name);
}

bool CutscenesContainer::check_exist(std::string const& cutscene_name) const
{
    if(return_cs_it(cutscene_name) != m_map.cend()) return true;
    else return false;
}
