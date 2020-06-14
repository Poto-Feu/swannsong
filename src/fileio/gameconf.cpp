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
#include "perror.h"
}

#include <fstream>
#include "gameconf.hpp"
#include "stringsm.h"

namespace gameconf
{
    bool splitins(std::string& var, std::string& value, std::string ins)
    {
        int index = 0;
        int str_size = static_cast<int>(ins.size());
        bool equal_fnd = false;
        bool quoteinc = false;
        bool val_finished = false;
        bool correct_syntax = false;

        for(int i = 0; i < str_size && !equal_fnd; ++i)
        {
            if(ins[i] == '=') equal_fnd = true;
            else var += ins[i];

            index = i+1;
        }

        if(!equal_fnd) perror_disp("cannot find var in gameconf line", false);

        for(int i = index; i < str_size && equal_fnd && !val_finished; ++i)
        {
            if(quoteinc == 1)
            {
                if(ins[i] == '"') val_finished = true;
                else value += ins[i];
            } else if(ins[i] == '"') quoteinc = true;
        }
        correct_syntax = val_finished;
        return correct_syntax;

    }

    /*Read data contained in the gameconf file and set the gameconf variable to the appropriate
    value*/
    std::vector<gcvar_struct> readfile(std::filesystem::path const& data_path)
    {
        std::ifstream gc_stream(data_path.string() + "gameconf.txt");

        if(!gc_stream.good()) {
            perror_disp(
                    "gameconf file not found (this may also applies to other game files)",
                    true);
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

                if(gameconf::splitins(var, value, curr_line))
                {
                    rtrn_vec.push_back(gcvar_struct {var, value});
                } else perror_disp("incorrect gameconf syntax", false);
            }
            curr_line.clear();
        }
        return rtrn_vec;
    }
}
