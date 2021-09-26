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

#include <ctime>
#include <fstream>

#include "game_error.hpp"
#include "stringsm.hpp"

static bool is_filepath_already_set = false;
static bool is_debug = false;
static bool has_error = false;

static std::string log_file_path;
static std::string fatal_error_msg;

void game_error::setdebug()
{
    is_debug = true;
}

std::string const& game_error::get_fatal_error_msg()
{
    return fatal_error_msg;
}

bool game_error::has_encountered_fatal()
{
    return has_error;
}

void game_error::fatal_error(std::string const& p_text)
{
    has_error = true;
    fatal_error_msg = p_text;
}

void game_error::log_write(std::string const& p_text)
{
    if(is_debug) {
        if(!log_file_path.empty()) {
            std::ofstream log_stream(log_file_path, std::ios::app);

            log_stream << p_text << std::endl;
        } else {
            fatal_error("log file path not set (game_error::set_filepath must "
                    "be run!)");
        }
    }
}

void game_error::log_write(std::vector<std::string> p_vector)
{
    if(is_debug) {
        for(auto const& it : p_vector) log_write(it);
    }
}

void game_error::emit_warning(std::string const& p_text)
{
    if(is_debug) {
        std::ofstream log_stream(log_file_path, std::ios::app);

        log_stream << "WARNING: ";
        log_stream.close();
        log_write(p_text);
    }
}

void game_error::set_filepath(std::string const& local_data_path)
{
    if(is_debug) {
        const unsigned int buf_size = 30;
        std::string log_intro;
        std::ofstream file_stream;
        auto time_var = std::time(nullptr);
        auto curr_time = *std::localtime(&time_var);
        char time_buffer[buf_size] = "\0";

        if(is_filepath_already_set) {
            emit_warning("game_error::files_path used twice");
        }

        std::strftime(time_buffer, buf_size - 1, "%Y-%m-%d_%H-%M-%S",
                &curr_time);

        log_file_path = local_data_path;

        log_file_path += "/";
        log_file_path += "log.txt";

        file_stream.open(log_file_path);

        log_intro = "[";
        log_intro += time_buffer;
        log_intro += "]\n\n";

        file_stream << log_intro;

        is_filepath_already_set = true;
    }
}
