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
    along with SwannSong Adventure.
    If not, see <https://www.gnu.org/licenses/>.
*/

#include <fstream>

#include "gameconf.hpp"
#include "fileio/conf_files.hpp"
#include "game_error.hpp"
#include "stringsm.hpp"

namespace gameconf
{
    /*Read data contained in the gameconf file and set the gameconf variable to the appropriate
    value*/
    std::vector<gcvar_struct> readfile(std::string const& data_path)
    {
        using namespace game_error;

        std::ifstream gc_stream(data_path + "gameconf.txt");

        if(!gc_stream.good()) {
            fatal_error("gameconf file not found (this may also applies to other game files)");
        }

        std::vector<gcvar_struct> rtrn_vec;
        std::string curr_line;

        while(std::getline(gc_stream, curr_line)) {
            stringsm::rtab(curr_line);
            if(curr_line.empty()) continue;
            if(curr_line.at(0) != '#')
            {
                std::string var;
                std::string value;

                if(conf_files::split_var(var, value, curr_line)) {
                    rtrn_vec.push_back(gcvar_struct {var, value});
                } else emit_warning("incorrect gameconf syntax");
            }
            curr_line.clear();
        }
        return rtrn_vec;
    }
}
