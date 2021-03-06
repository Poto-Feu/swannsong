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
#include <string>
#include <vector>
#include "pstrings.h"
#include "fileio/fileio.h"
#include "files_path.hpp"
#include "game_error.hpp"
#include "lang.hpp"
#include "stringsm.h"

namespace pstrings
{
    struct PstringsElement
    {
        std::string id;
        std::string val;
    };

    static std::vector<PstringsElement> pstr_vec {};

    //Set the file pointer to the file containing the strings correponding to the selected language
    static std::ifstream open_strfile(std::string p_langdir)
    {
        p_langdir.append(langmod::get_lang());
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

        if(!quote_inc) game_error::fatal_error("wrong pstring format(\"" + buf + "\"");

        for(int i = quote_ind+1; buf[i] != '\0'; ++i)
        {
            if(buf[i] == quote_ch) break;
            else if(buf[i] == '\\' && buf[i+1] == quote_ch) {
                r_val += quote_ch;
                ++i;
            } else r_val += buf[i];
        }
    }

    static void add_to_vec(std::string const& p_id, std::string const& p_val)
    {
        pstr_vec.push_back({std::move(p_id), std::move(p_val)});
    }

    void copy_file_to_vec(std::string const& p_langdir, std::filesystem::path const& data_path)
    {
        using namespace files_path;

        std::string buf;
        std::ifstream file_stream = open_strfile(data_path.string() + p_langdir);

        while(fileio::getfileln(buf, file_stream)) {
            std::string r_id;
            std::string r_val;

            stringsm::rtab(buf);

            if(!buf.empty() && buf[0] != '#') {
                split_file_line(r_id, r_val, buf);
                add_to_vec(r_id, r_val);
            } else continue;
        }
    }

    static auto find_it_vec(std::string const& p_id)
    {
        return std::find_if(pstr_vec.cbegin(), pstr_vec.cend(),
                [p_id](PstringsElement const& cid) {
                return p_id == cid.id;
                });
    }

    //Copy the corresponding string into the pointer of a char pointer
    std::string fetch(std::string const& p_id)
    {
        auto it = find_it_vec(p_id);

        if(it != pstr_vec.cend()) return it->val;
        else return "MissingStr";
    }

    //Check if a string is defined in the lang file
    bool check_exist(std::string const& p_id)
    {
        return find_it_vec(p_id) != pstr_vec.cend();
    }
}
