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
#include <string>
#include <unordered_map>

/*The corresponding value is the default value of the variable if it was not defined in the conf
file*/
const std::unordered_map<std::string, std::string> local_conf_vars({
        {"firstlaunch", "1"},
        {"lang", "eng"}
});

//Class containing the variables defined in the local configuration file
class LocalConfVariableContainer
{
    public:

        LocalConfVariableContainer();
        explicit LocalConfVariableContainer(std::string const& local_data_path);

        std::string const& getValue(std::string const& variable_name);
        bool changeValue(std::string const& variable_name, std::string const& value);
        bool writeToFile() const;
        void deleteFile() const;
        void reset();

    private:

        std::unordered_map<std::string, std::string> m_map;
        std::filesystem::path m_file_path;
};
