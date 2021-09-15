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
    along with SwannSong Adventure.
    If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef PSTRINGS_HPP
#define PSTRINGS_HPP

#include <filesystem>
#include <string>
#include <unordered_map>

typedef std::string PStringsElement;

//Stores program strings
class PStrings
{
    public:

        PStrings();
        PStrings(std::string const& lang_code, std::string const& langdir,
                std::filesystem::path const& data_path);

        std::string const& fetch(std::string const& id) const;
        bool check_exist(std::string const& id) const;

    private:

        std::unordered_map<std::string, PStringsElement> m_map;

        //Return an iterator corresponding to p_id key
        auto find_it_vec(std::string const& id) const;
};
#endif
