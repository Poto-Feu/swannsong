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

extern "C" {
#include <curses.h>
#include "perror.h"
}

#include <fstream>
#include "cutscenes.hpp"
#include "fileio/fileio.h"
#include "vars/pvars.hpp"
#include "pcurses.hpp"
#include "pstrings.h"
#include "stringsm.h"
#include "userio.h"

Cutscene::Cutscene() { }

namespace cutscenes
{
    std::vector<Cutscene> vec;

    /*Initialize the vector by reading the cutscenes file*/
    void copy_file_to_vec()
    {
        bool in_cutscene = false;
        std::string buf;
        std::string cs_file = pvars::getstdvars("csfile");
        std::ifstream file_stream (pvars::getstdvars("csfile"));

        while(fileio::getfileln(buf, file_stream))
        {
            Cutscene curr_cs;

            stringsm::rtab(buf);

            if(buf == "" || buf[0] == '#') continue;

            in_cutscene = true;
            curr_cs.name = stringsm::getfw(buf);

            while(fileio::getfileln(buf, file_stream) && in_cutscene)
            {
                std::string fw;
                cs_action curr_action;

                stringsm::rtab(buf);
                fw = stringsm::getfw(buf);

                if(fw == "PAUSE") curr_action.type = cs_action_type::PAUSE;
                else if(fw == "BLANK") curr_action.type = cs_action_type::BLANK;
                else if(stringsm::is_str(buf))
                {
                    curr_action.type = cs_action_type::STRING;
                    curr_action.content = stringsm::ext_str_quotes(buf);
                } else if(pstrings::check_exist(fw))
                {
                    curr_action.type = cs_action_type::STRING;
                    curr_action.content = pstrings::fetch(fw);
                } else if(fw == "END")
                {
                    in_cutscene = false;
                    break;
                }
                curr_cs.vec.push_back(curr_action);
            }
            vec.push_back(curr_cs);
        }
    }

    /*Display a cutscene*/
    void display(std::string const p_name)
    {
        const int y_start = 3;
        bool is_fnd = false;

        for(auto const& it : vec)
        {
            if(it.name == p_name)
            {
                clear();
                is_fnd = true;
                move(y_start, pcurses::margin);

                for(auto const& curr_action: it.vec)
                {
                    if(curr_action.type == cs_action_type::PAUSE)
                    {
                        move(LINES - 3, pcurses::margin);
                        pstrings::display("continue_penter");
                        userio::waitenter();
                        clear();
                        move(y_start, pcurses::margin);
                    } else if (curr_action.type == cs_action_type::BLANK)
                    {
                        printw("\n");
                    } else
                    {
                        pcurses::display_center_string(curr_action.content);
                        printw("\n");
                    }
                }
                move(LINES - 3, pcurses::margin);
                pstrings::display("continue_penter");
                userio::waitenter();
                clear();
            }
        }

        if(!is_fnd)
        {
            std::string err_str("cutscene not found(" + p_name + ")");

            perror_disp(err_str.c_str(), true);
        }
    }

    bool check_exist(std::string const p_name)
    {
        for(auto const& it : vec)
        {
            if(it.name == p_name) return true;
        }
        return false;
    }
}
