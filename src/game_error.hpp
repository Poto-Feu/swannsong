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
    along with SwannSong Adventure.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef GAME_ERROR_HPP
#define GAME_ERROR_HPP

#include <string>
#include <vector>

namespace game_error
{
    void setdebug();
    std::string const& get_fatal_error_msg();
    bool has_encountered_fatal();
    void log_write(std::string const& p_text);
    void log_write(std::vector<std::string> p_vector);
    void emit_warning(std::string const& p_text);
    void fatal_error(std::string const& p_text);
    void set_filepath(std::string const& local_data_path);
}
#endif
