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

#include "vars/LocalConfVariable.hpp"
#include "fileio/conf_files.hpp"
#include "fileio/fileio.h"
#include "game_error.hpp"
#include "stringsm.h"

static const std::string LOCAL_CONF_FILENAME = "conf_vars.txt";

LocalConfVariableContainer::LocalConfVariableContainer() { }
LocalConfVariableContainer::LocalConfVariableContainer(std::string const& local_data_path) :
    m_file_path(local_data_path + LOCAL_CONF_FILENAME)
{
    if(fileio::file_exists(m_file_path)) {
        std::vector<std::string> file_content = fileio::copy_to_vector(m_file_path);

        for(auto& it : file_content) {
            stringsm::rtab(it);

            if(it == "" || it[0] == '#') continue;

            std::string var;
            std::string value;
            if(!conf_files::split_var(var, value, it)) {
                game_error::emit_warning("Wrong variable formatting: " + it);
                continue;
            }
            m_map[var] = value;
        }

        for(auto const& it : local_conf_vars) {
            if(!m_map.count(it.first)) m_map[it.first] = it.second;
        }
    } else {
        m_map = local_conf_vars;
        writeToFile();
    }
}

std::string const& LocalConfVariableContainer::getValue(std::string const& variable_name)
{
    if(m_map.find(variable_name) == m_map.cend()) return stringsm::empty_string;
    else return m_map[variable_name];
}

bool LocalConfVariableContainer::changeValue(std::string const& variable_name,
        std::string const& value)
{
    if(m_map.find(variable_name) == m_map.end()) return false;

    m_map[variable_name] = value;
    return true;
}

bool LocalConfVariableContainer::writeToFile() const
{
    std::ofstream file_stream(m_file_path);

    for(auto const& it : m_map) {
        file_stream << it.first << "=" << "\"" << it.second << "\"" << std::endl;
    }
    return true;
}

void LocalConfVariableContainer::deleteFile() const
{
    std::filesystem::remove(m_file_path);
}

void LocalConfVariableContainer::reset()
{
    m_map = local_conf_vars;
}
