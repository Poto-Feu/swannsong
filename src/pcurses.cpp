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

#include "pcurses.hpp"
#include "display_server.hpp"
#include "userio.h"

namespace pcurses
{
    int margin = 0;
    int title_y = 0;
    int lines = 0;
    int cols = 0;

    unsigned int max_size_str()
    {
        return COLS - margin * 2;
    }

    static void multiline_center_string(std::string const& p_str, int startline, int p_attr)
    {
        bool end_of_str = false;
        std::string remain_str = p_str;
        std::vector<std::string> str_vec;

        while(!end_of_str) {
            std::string vec_item;

            if(remain_str.size() > max_size_str()) {
                vec_item = remain_str.substr(0, max_size_str());
                remain_str.erase(0, max_size_str());
                str_vec.push_back(vec_item);
            } else end_of_str = true;
        }

        for(auto const& vec_it : str_vec) {
            display_server::add_string(vec_it, {startline, pcurses::margin}, p_attr);
            ++startline;
        }

        display_server::add_string(remain_str, {startline, pcurses::margin});
    }

    int find_centered_x(std::string const& p_str)
    {
        return COLS / 2 - static_cast<int>(p_str.size()) / 2;
    }

    void display_pos_string(std::string p_str, int x_space, int startline, int p_attr)
    {
        const int multiline_space = 3;
        bool end_of_str = false;
        bool end_of_zone = false;
        unsigned int pos_str_max_size = max_size_str() / 2 + x_space;

        for(auto i = 0; !end_of_str && !end_of_zone; ++i) {
            unsigned int str_size = p_str.size();

            if(startline > lines - 5) end_of_zone = true;
            else if(str_size > pos_str_max_size) {
                std::string curr_str = p_str.substr(0, pos_str_max_size);

                p_str.erase(0, pos_str_max_size);
                display_server::add_string(curr_str, {startline, cols / 2 - x_space}, p_attr);
                ++startline;
            } else {
                display_server::add_string(p_str, {startline, cols / 2 - x_space}, p_attr);
                end_of_str = true;
            }

            if(i == 0) {
                x_space += multiline_space;
                pos_str_max_size -= multiline_space;
            }
        }
    }

    void display_center_string(std::string const& p_str, int startline, int p_attr)
    {
        unsigned int max_size_func = max_size_str();

        if(p_str.size() < max_size_func) {
            int p_x = find_centered_x(p_str);

            display_server::add_string(p_str, {startline, p_x}, p_attr);
        } else multiline_center_string(p_str, startline, p_attr);
    }

    void display_penter_message(PStrings const& program_strings)
    {
            display_server::add_string(program_strings.fetch("continue_penter"),
                    {pcurses::lines - pcurses::bottom_margin, pcurses::margin}, A_BOLD);
            display_server::show_screen();
            userio::waitenter();
    }
}
