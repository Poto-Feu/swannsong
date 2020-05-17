/*
    Copyright (C) 2020 Adrien Saad

    This file is part of SwannSong.

    SwannSong is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SwannSong is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SwannSong.  If not, see <https://www.gnu.org/licenses/>.
*/

extern "C" 
{
#include <curses.h>
#include <string.h>
#include "fileio/fileio.h"
#include "perror.h"
}

#include <string>
#include <vector>
#include "pstrings.h"
#include "vars/pconst.hpp"
#include "vars/pvars.hpp"
#include "stringsm.h"

struct PstringsElement
{
    std::string id;
    std::string val;
};

static std::vector<PstringsElement> pstrings_arr {};

static void open_strfile(FILE **f);
static void split_file_line(std::string& r_id, std::string& r_val, char* buf);
static void add_pstring_to_vec(std::string p_id, std::string p_val);

void pstrings_copy_file_to_vec()
{
    FILE* fp = NULL;
    char buf[P_MAX_BUF_SIZE]{0};

    open_strfile(&fp);

    while(fileio_getfileln(buf, P_MAX_BUF_SIZE, &fp) != NULL)
    {
        std::string r_id;
        std::string r_val;

        stringsm_chomp(buf);
        stringsm_rtab(buf);

        if(buf[0] != '\0')
        {
            split_file_line(r_id, r_val, buf);
            add_pstring_to_vec(r_id, r_val);
        } else continue;
    }
    fclose(fp);
}

static void split_file_line(std::string& r_id, std::string& r_val, char* buf)
{
    int sp_ind = 0;
    int quote_ind = 0;
    bool quote_inc = false;

    for(int i = 0; buf[i] != '\0'; i++)
    {
        if(buf[i] == ' ' || buf[i] == '\t') break;
        sp_ind++;
        r_id += buf[i];
    }
    for(int i = sp_ind; buf[i] != '\0'; i++)
    {
        if(buf[i] == '"')
        {
            quote_inc = true;
            quote_ind = i;
            break;
        }
    }

    if(!quote_inc)
    {
        perror_disp("wrong pstring format", 1);
    }
    for(int i = quote_ind+1; buf[i] != '\0'; i++)
    {
        if(buf[i] == '"') break;
        else r_val += buf[i];
    }
}

static void add_pstring_to_vec(std::string p_id, std::string p_val)
{
    PstringsElement new_el = {p_id, p_val};
    pstrings_arr.push_back(new_el);
}

std::string pstrings_fetch(std::string const& id);

/*Display the string corresponding to the id*/
void pstrings_display(const char* id)
{
    std::string rstring = pstrings_fetch(id);

    printw(rstring.c_str());
}


/*Check if a string is defined in the lang file*/
bool pstrings_check_exist(const char* id)
{
    bool isfnd = false;
    auto str_id(id);

    for(const auto& it : pstrings_arr)
    {
        if(str_id == it.id)
        {
            isfnd = true;
            break;
        }
    }
    
    return isfnd;
}
/*Copy the corresponding string into the pointer of a char pointer*/
std::string pstrings_fetch(std::string const& id)
{
    bool isfnd = false;
    std::string r_str;

    for(const auto& it : pstrings_arr)
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

/*Set the file pointer to the file containing the strings correponding
to the selected language*/
static void open_strfile(FILE **f)
{
    char langfile_chr[P_MAX_BUF_SIZE] = {'\0'};
    std::string langdir = pvars::getgcvars("langdir");
    std::string lang = pvars::getstdvars("lang");
    std::string langfile;
   
    langfile = langdir;
    langfile.append(lang);
    langfile.append(".txt");

    strcpy(langfile_chr, langfile.c_str());

    fileio_setfileptr(f, langfile_chr);
}
