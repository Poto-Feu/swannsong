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
    along with SwannSong Adventure. If not, see
    <https://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include <fstream>

#include "pstrings.hpp"
#include "fileio/fileio.h"
#include "game_error.hpp"
#include "stringsm.h"
#include "terminal.hpp"

struct pstrings::ps_data {
    std::unordered_map<std::string, std::string> map;
};

static const std::string missing_pstring_str = "MissingString";

/* Set the file pointer to the file containing the strings correponding to
 * the selected language. */
static std::ifstream open_strfile(std::string const& lang_code,
        std::string p_langdir)
{
    p_langdir.append(lang_code);
    p_langdir.append(".txt");

    return std::ifstream(p_langdir);
}

static void split_file_line(std::string& r_id, std::string& r_val,
        std::string const& buf)
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

    if(!quote_inc) {
        game_error::emit_warning("wrong pstring format (" + buf + ")");
    } else {
        for(int i = quote_ind+1; buf[i] != '\0'; ++i) {
            if(buf[i] == quote_ch) {
                break;
            } else if(buf[i] == '\\' && buf[i+1] == quote_ch) {
                r_val += quote_ch;
                ++i;
            } else {
                r_val += buf[i];
            }
        }
    }
}

static bool read_pstrings_from_file(std::ifstream& file_stream,
        std::unordered_map<std::string, std::string>& map)
{
    std::string buf;

    while(fileio::getfileln(buf, file_stream)) {
        std::string r_id;
        std::string r_val;

        stringsm::rtab(buf);

        if(!buf.empty() && buf[0] != '#') {
            split_file_line(r_id, r_val, buf);
            if(game_error::has_encountered_fatal()) {
                return false;
            } else {
                map[r_id] = std::move(r_val);
            }
        }
    }

    return true;
}

pstrings::ps_data_ptr pstrings::init_data(std::string const& data_path,
        std::string const& lang_code)
{
    const std::string lang_dir = "lang/";
    std::ifstream file_stream;
    pstrings::ps_data_ptr pstrings_data(new pstrings::ps_data);

    terminal::set_locale(lang_code);

    file_stream = open_strfile(lang_code, data_path + lang_dir);

#ifdef GAME_VERSION
    pstrings_data->map["GAME_VERSION"] = "v" + std::string(GAME_VERSION);
#else
    pstrings_data->map["GAME_VERSION"] = "UndefinedGAME_VERSION";
#endif

#ifdef GAME_NAME
    pstrings_data->map["GAME_NAME"] = GAME_NAME;
#else
    pstrings_data->map["GAME_NAME"] = "UndefinedGAME_NAME";
#endif
    if(!read_pstrings_from_file(file_stream, pstrings_data->map)) {
        return nullptr;
    } else {
        return pstrings_data;
    }
}

bool pstrings::check_if_exists(pstrings::ps_data_ptr const& pstrings_data,
        std::string const& id)
{
    return pstrings_data->map.find(id) != pstrings_data->map.cend();
}

std::string const& pstrings::fetch_string(pstrings::ps_data_ptr const& data,
        std::string const& id)
{
    auto const pstrings_iterator = data->map.find(id);

    if(pstrings_iterator == data->map.cend()) {
        return missing_pstring_str;
    } else {
        return pstrings_iterator->second;
    }
}
