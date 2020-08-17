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
}

#include <fstream>
#include <iostream>
#include "game_error.hpp"
#include "exitgame.h"
#include "files_path.hpp"
#include "stringsm.h"

namespace game_error
{
    namespace fs = std::filesystem;

    static std::filesystem::path log_file_path;
    static bool is_debug = false;

    static bool has_error = false;
    static std::string fatal_error_msg;

    void setdebug()
    {
        is_debug = true;
    }

    std::string const& get_fatal_error_msg()
    {
        return fatal_error_msg;
    }

    bool has_encountered_fatal()
    {
        return has_error;
    }

    void fatal_error(std::string const& p_text)
    {
        has_error = true;
        fatal_error_msg = p_text;
    }

    void log_write(std::string const& p_text)
    {
        if(is_debug) {
            if(!log_file_path.empty()) {
                std::ofstream log_stream(log_file_path, std::ios::app);

                log_stream << p_text << std::endl;
            } else fatal_error("log file path not set (game_error::set_filepath must be run!)");
        }
    }

    void log_write(std::vector<std::string> p_vector)
    {
        if(is_debug) {
            for(auto const& it : p_vector) log_write(it);
        }
    }

    void emit_warning(std::string const& p_text)
    {
        if(is_debug) {
            std::ofstream log_stream(log_file_path, std::ios::app);

            log_stream << "WARNING: ";
            log_stream.close();
            log_write(p_text);
        }
    }

    void set_filepath(fs::path const& local_data_path)
    {
        if(is_debug) {
            static bool already_used = false;

            if(!already_used) {
                auto remove_previous_logs = [&]() {
                    fs::directory_iterator it_end;

                    for(fs::directory_iterator it(log_file_path); it != it_end; ++it) {
                        if(fs::is_regular_file(it->status())
                                && stringsm::to_lower(it->path().extension().string()) == ".txt") {
                            fs::remove(it->path());
                        }
                    }
                };

                const unsigned int buf_size = 30;
                auto time_var = std::time(nullptr);
                auto curr_time = *std::localtime(&time_var);
                char time_buffer[buf_size] = "\0";

                strftime(time_buffer, buf_size - 1, "%Y-%m-%d_%H-%M-%S", &curr_time);

                log_file_path = local_data_path;
                log_file_path += "logs";
                files_path::create_directory(log_file_path);
                remove_previous_logs();
                log_file_path += "/";
                log_file_path += time_buffer;
                log_file_path += "_log.txt";

                std::string log_intro = "[";

                log_intro += time_buffer;
                log_intro += "]";
                
                log_write({log_intro,
                        "These first two lines are for testing purposes.",
                        "They can be ignored.",
                        ""});
                already_used = true;
            } else {
                emit_warning("game_error::files_path used twice - this might cause problems later");
            }
        }
    }
}
