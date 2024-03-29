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

#include "pcurses.hpp"
#include "stringsm.hpp"

int pcurses::margin = 0;
int pcurses::title_y = 0;
int pcurses::lines = 0;
int pcurses::cols = 0;

static size_t max_size_str()
{
    return (size_t)(COLS - pcurses::margin * 2);
}

static unsigned int multiline_center_string(std::string const& p_str,
        int startline, int p_attr)
{
    bool end_of_str = false;
    unsigned int line_count = 0;
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
        display_server::add_string(vec_it, {startline, pcurses::margin},
                p_attr);
        ++startline;
        ++line_count;
    }

    display_server::add_string(remain_str, {startline, pcurses::margin});
    return ++line_count;
}

static int find_centered_x(std::string const& p_str)
{
    return COLS / 2 - static_cast<int>(p_str.size()) / 2;
}

void pcurses::init()
{
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);

    if(COLS < 100) {
        pcurses::margin = 4;
    } else if(COLS > 200) {
        pcurses::margin = 15;
    } else {
        pcurses::margin = 10;
    }

    pcurses::title_y = LINES / 2 - LINES / 6;
    pcurses::lines = LINES;
    pcurses::cols = COLS;
}

void pcurses::clean()
{
    delwin(stdscr);
    endwin();
}

/*Pause the program until the user press Enter*/
void pcurses::waitenter()
{
    while(!pcurses::is_enter_key(getch())) {}
}

void pcurses::display_pos_string(std::string p_str, int x_space, int startline,
        int p_attr)
{
    const int multiline_space = 3;
    bool end_of_str = false;
    bool end_of_zone = false;
    size_t pos_str_max_size = max_size_str() / 2 + (size_t)x_space;

    for(auto i = 0; !end_of_str && !end_of_zone; ++i) {
        const size_t str_size = p_str.size();

        if(startline > lines - 5) end_of_zone = true;
        else if(str_size > pos_str_max_size) {
            std::string curr_str = p_str.substr(0, pos_str_max_size);

            p_str.erase(0, pos_str_max_size);
            display_server::add_string(curr_str,
                    {startline, cols / 2 - x_space}, p_attr);
            ++startline;
        } else {
            display_server::add_string(p_str, {startline, cols / 2 - x_space},
                    p_attr);
            end_of_str = true;
        }

        if(i == 0) {
            x_space += multiline_space;
            pos_str_max_size -= multiline_space;
        }
    }
}

std::vector<std::string> pcurses::divide_string_into_lines(std::string p_string)
{
    if(p_string.size() < max_size_str()) {
        return std::vector<std::string> { std::move(p_string) };
    } else {
        bool end_of_str = false;
        std::vector<std::string> str_vec;

        while(!end_of_str) {
            std::string vec_item;

            if(p_string.size() > max_size_str()) {
                vec_item = p_string.substr(0, max_size_str());
                p_string.erase(0, max_size_str());
                str_vec.push_back(std::move(vec_item));
            } else end_of_str = true;
        }
        str_vec.push_back(std::move(p_string));
        return str_vec;
    }
}

unsigned int pcurses::display_center_string(std::string const& p_str, int startline, int p_attr)
{
    const size_t max_size_func = max_size_str();

    if(p_str.size() < max_size_func) {
        int p_x = find_centered_x(p_str);

        display_server::add_string(p_str, {startline, p_x}, p_attr);
        return 1;
    } else return multiline_center_string(p_str, startline, p_attr);
}

void pcurses::display_penter_message(
        pstrings::ps_data_ptr const& pstrings_data, bool wait_enter)
{
        display_server::add_string(
                pstrings::fetch_string(pstrings_data, "continue_penter"),
                {pcurses::lines - pcurses::bottom_margin, pcurses::margin},
                A_BOLD);
        display_server::show_screen();
        if(wait_enter) {
            waitenter();
        }
}

std::string pcurses::get_text_input(int max_n)
{
    std::string input_str;
    char* buf = new char[(size_t)max_n+1];

    buf[max_n] = '\0';

    echo();
    getnstr(buf, max_n);
    noecho();

    stringsm::chomp(buf);
    input_str.assign(buf);
    delete[](buf);

    return input_str;
}

bool pcurses::is_enter_key(int key)
{
#ifdef _WIN32
    const int enter_key = 13;
#else
    const int enter_key = '\n';
#endif

    return key == enter_key;
}
