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
    along with SwannSong Adventure.  If not, see
    <https://www.gnu.org/licenses/>.
*/

#include "dialogbox.hpp"
#include "pcurses.hpp"
#include "userio.hpp"

namespace dialogbox
{
    static void display_strings(std::vector<std::string> const& p_strings,
            const unsigned int first_line_index, unsigned int start_line, unsigned int max_lines,
            pstrings::ps_data_ptr const& pstrings_data)
    {
        size_t i = first_line_index;
        for( ; i < p_strings.size(); ++i) {
            if(i - first_line_index >= max_lines) break;
            else {
                pcurses::display_center_string(p_strings[i], start_line);
                start_line = display_server::get_last_line() + 1;
            }
        }

        if(p_strings.size() - first_line_index >= max_lines + 1) {
            pcurses::display_center_string(
                    pstrings::fetch_string(pstrings_data, "dialogbox_more"),
                    LINES - 5);
        }
    }

    void display(const std::string *title, const std::vector<std::string> *p_strings,
            pstrings::ps_data_ptr const& pstrings_data)
    {
        bool screen_exited = false;
        //Keep track of the scrolling state
        unsigned int strings_display_offset = 0;

        while(!screen_exited) {
            const unsigned int max_content_lines = pcurses::lines - 8;
            const unsigned int title_line = pcurses::top_margin;
            unsigned int content_start_line = title_line;
            unsigned int current_max_content_lines = max_content_lines;

            display_server::clear_screen();

            if(title) {
                pcurses::display_center_string(*title, title_line, A_BOLD);
                content_start_line = display_server::get_last_line() + 2;
                current_max_content_lines = max_content_lines + title_line - content_start_line;
            }

            size_t strings_n_lines = 0;
            if(p_strings) {
                std::vector<std::string> vec_lines;
                for(auto const& it : *p_strings) {
                    std::vector<std::string> itVec = pcurses::divide_string_into_lines(it);
                    vec_lines.insert(vec_lines.end(), itVec.begin(), itVec.end());
                }
                strings_n_lines = vec_lines.size();
                display_strings(vec_lines, strings_display_offset, content_start_line,
                        current_max_content_lines, pstrings_data);
            }
            pcurses::display_penter_message(pstrings_data, false);

            bool is_key_pressed = false;
            while(!is_key_pressed) {
                int key_pressed = getch();

                if(key_pressed == KEY_UP && strings_display_offset > 0 && p_strings) {
                    strings_display_offset -= 1;
                    is_key_pressed = true;
                } else if(key_pressed == KEY_DOWN && p_strings) {
                    if(strings_n_lines - strings_display_offset > current_max_content_lines) {
                        ++strings_display_offset;
                        is_key_pressed = true;
                    }
                } else if(pcurses::is_enter_key(key_pressed)) {
                    is_key_pressed = true;
                    screen_exited = true;
                }
            }
        }
    }
}
