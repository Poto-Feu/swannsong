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

#include <fstream>

#include "pstrings.hpp"
#include "fileio/fileio.h"
#include "files_path.hpp"
#include "game_error.hpp"
#include "stringsm.h"

//Set the file pointer to the file containing the strings correponding to the selected language
static std::ifstream open_strfile(std::string const& lang_code, std::string p_langdir)
{
    p_langdir.append(lang_code);
    p_langdir.append(".txt");

    return std::ifstream(p_langdir);
}

static void split_file_line(std::string& r_id, std::string& r_val, std::string const& buf)
{
    int sp_ind = 0;
    int quote_ind = 0;
    bool quote_inc = false;
    char quote_ch = '\0';

    for(const auto& it : buf) {
        if(it == ' ' || it == '\t') break;
        else {
            ++sp_ind;
            r_id += it;
        }
    }

    for(int i = sp_ind; buf[i] != '\0'; ++i) {
        if(buf[i] == '"' || buf[i] == '\'') {
            quote_inc = true;
            quote_ch = buf[i];
            quote_ind = i;
            break;
        }
    }

    if(!quote_inc) game_error::fatal_error("wrong pstring format (" + buf + ")");
    else {
        for(int i = quote_ind+1; buf[i] != '\0'; ++i) {
            if(buf[i] == quote_ch) break;
            else if(buf[i] == '\\' && buf[i+1] == quote_ch) {
                r_val += quote_ch;
                ++i;
            } else r_val += buf[i];
        }
    }
}

auto PStrings::find_it_vec(std::string const& id) const
{
    return m_map.find(id);
}

//Check if a string is defined in the lang file
bool PStrings::check_exist(std::string const& id) const
{
    return find_it_vec(id) != m_map.cend();
}

PStrings::PStrings() { }
PStrings::PStrings(std::string const& lang_code, std::string const& langdir,
        std::string const& data_path)
{
    using namespace files_path;

    std::string buf;
    std::ifstream file_stream = open_strfile(lang_code, data_path + langdir);

    /*THIS MUST NOT BE REMOVED ! It will serve as a placeholder if the program string was not
    defined*/
    m_map["missing_str"] = "MissingString";
#ifdef GAME_VERSION
    m_map["GAME_VERSION"] = "v" + std::string(GAME_VERSION);
#else
    m_map["GAME_VERSION"] = "UndefinedGAME_VERSION";
#endif

#ifdef GAME_NAME
    m_map["GAME_NAME"] = GAME_NAME;
#else
    m_map["GAME_NAME"] = "UndefinedGAME_NAME";
#endif

    while(fileio::getfileln(buf, file_stream)) {
        std::string r_id;
        std::string r_val;

        stringsm::rtab(buf);

        if(!buf.empty() && buf[0] != '#') {
            split_file_line(r_id, r_val, buf);
            if(game_error::has_encountered_fatal()) return;
            else m_map[r_id] = std::move(r_val);

        } else continue;
    }
}

std::string const& PStrings::fetch(std::string const& id) const
{
    auto string_it = find_it_vec(id);

    if(string_it != m_map.cend()) return string_it->second;
    else return m_map.find("missing_str")->second;
}
