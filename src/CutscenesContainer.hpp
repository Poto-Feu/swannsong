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

#ifndef CUTSCENES_CONTAINER_HPP
#define CUTSCENES_CONTAINER_HPP

#include <unordered_map>
#include <filesystem>

#include "CutsceneClass.hpp"

class CutscenesContainer
{
    public:

        CutscenesContainer();
        CutscenesContainer(std::string const& csfile,
                std::filesystem::path const& data_path,
                PStrings const& program_strings);

        // Return nullptr if the cutscene does not exists
        Cutscene const* get_cutscene(std::string const& name) const;

    private:

        std::unordered_map<std::string, Cutscene>::const_iterator return_cs_it(
                std::string const& cutscene_name) const;
        bool check_exist(std::string const& cutscene_name) const;

        std::unordered_map<std::string, Cutscene> m_map;
};
#endif
