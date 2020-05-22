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

extern "C"  {
#include <curses.h>
#include "perror.h"
}

#include <string>
#include <vector>
#include "pstrings.h"
#include "fileio/fileio.h"
#include "vars/pconst.hpp"
#include "vars/pvars.hpp"
#include "stringsm.h"

namespace pstrings
{
    struct PstringsElement
    {
        std::string id;
        std::string val;
    };

    static std::vector<PstringsElement> arr {};

    /*Set the file pointer to the file containing the strings correponding
    to the selected language*/
    static std::ifstream open_strfile()
    {
        std::string langdir = pvars::getgcvars("langdir");
        std::string lang = pvars::getstdvars("lang");
        std::string langfile;
       
        langfile = langdir;
        langfile.append(lang);
        langfile.append(".txt");

        return std::ifstream(langfile);
    }

    static void split_file_line(std::string& r_id, std::string& r_val,
            std::string buf)
    {
        int sp_ind = 0;
        int quote_ind = 0;
        bool quote_inc = false;
        char quote_ch = '\0';

        for(const auto& it : buf)
        {
            if(it == ' ' || it == '\t') break;
            else
            {
                ++sp_ind;
                r_id += it;
            }
        }

        for(int i = sp_ind; buf[i] != '\0'; ++i)
        {
            if(buf[i] == '"' || buf[i] == '\'')
            {
                quote_inc = true;
                quote_ch = buf[i];
                quote_ind = i;
                break;
            }
        }

        if(!quote_inc) perror_disp("wrong pstring format", true);

        for(int i = quote_ind+1; buf[i] != '\0'; ++i)
        {
            if(buf[i] == quote_ch) break;
            else if(buf[i] == '\\' && buf[i+1] == quote_ch)
            {
                r_val += quote_ch;
                ++i;
            }
            else r_val += buf[i];
        }
    }

    static void add_to_vec(std::string p_id, std::string p_val)
    {
        PstringsElement new_el = {p_id, p_val};
        arr.push_back(new_el);
    }

    void copy_file_to_vec()
    {
        std::string buf;
        std::ifstream file_stream = open_strfile();

        while(fileio::getfileln(buf, file_stream))
        {
            std::string r_id;
            std::string r_val;

            stringsm::rtab(buf);

            if(!buf.empty())
            {
                split_file_line(r_id, r_val, buf);
                add_to_vec(r_id, r_val);
            } else continue;
        }
    }

    /*Copy the corresponding string into the pointer of a char pointer*/
    std::string fetch(std::string const id)
    {
        bool isfnd = false;
        std::string r_str;

        for(const auto& it : arr)
        {
            if(id == it.id)
            {
                isfnd = true;
                r_str = it.val;
                break;
            }
        }

        if(!isfnd) return "MissingStr";
        else return r_str;
    }

    void display(std::string const id)
    {
        std::string rstring = fetch(id);

        printw(rstring.c_str());
    }

    /*Check if a string is defined in the lang file*/
    bool check_exist(std::string const id)
    {
        bool isfnd = false;
        auto str_id(id);

        for(const auto& it : pstrings::arr)
        {
            if(str_id == it.id)
            {
                isfnd = true;
                break;
            }
        }
        return isfnd;
    }
}
