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

static void execute_all_actions(std::vector<cs_action> const& p_vec)
{
    std::string penter_msg = pstrings::fetch("continue_penter");

    for(auto const& action_it : p_vec)
    {
        switch(action_it.type)
        {
            case cs_action_type::STRING:
                pcurses::display_center_string(action_it.content);
                printw("\n");
                break;
            case cs_action_type::BLANK:
                printw("\n");
                break;
            case cs_action_type::PAUSE:
                move(LINES - 3, pcurses::margin);
                printw("%s", penter_msg.c_str());
                userio::waitenter();
                clear();
                refresh();
                move(pcurses::top_margin, pcurses::margin);
                break;
        }

        refresh();
    }

    move(LINES - 3, pcurses::margin);
    printw("%s", penter_msg.c_str());
    userio::waitenter();
    clear();
}

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
                curr_cs.actions_vec.push_back(curr_action);
            }
            vec.push_back(curr_cs);
        }
    }

    //Display a cutscene
    void display(std::string const& p_name)
    {
        clear();
        move(pcurses::top_margin, pcurses::margin);

        for(auto const& cs_it : vec)
        {
            if(cs_it.name == p_name)
            {
                execute_all_actions(cs_it.actions_vec);
                return;
            }
        }

        std::string penter_msg = pstrings::fetch("continue_penter");

        pcurses::display_center_string("missingCutscene");
        move(LINES - 3, pcurses::margin);
        printw(penter_msg.c_str());
        userio::waitenter();
        refresh();
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
