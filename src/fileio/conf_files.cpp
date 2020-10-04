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

#include "fileio/conf_files.hpp"
#include "game_error.hpp"

namespace conf_files
{
    bool split_var(std::string& var, std::string& value, std::string const& ins)
    {
        size_t index = 0;
        size_t str_size = ins.size();
        bool equal_fnd = false;
        bool quoteinc = false;
        bool val_finished = false;

        for(size_t i = 0; i < str_size && !equal_fnd; ++i) {
            if(ins[i] == '=') equal_fnd = true;
            else var += ins[i];
            index = i+1;
        }

        if(!equal_fnd) {
            game_error::emit_warning("cannot find var in gameconf line");
            return false;
        }

        for(; index < str_size && equal_fnd && !val_finished; ++index)
        {
            if(quoteinc) {
                if(ins[index] == '"') val_finished = true;
                else value += ins[index];
            } else if(ins[index] == '"') quoteinc = true;
        }

        return val_finished;
    }
}
