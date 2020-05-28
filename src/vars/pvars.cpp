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

#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "pvars.hpp"

namespace pvars
{
    static const std::string pvars_undef = "undefined";

    static std::vector<pvar_struct> stdvars = {
        {"lang", pvars_undef},
        {"csfile", pvars_undef}
    };

    static bool fetch_pvarsid(std::string const& p_name, int& p_id)
    {
        bool isvarfnd = false;
        int varfndid = -1;
        int arrlen = stdvars.size();

        for(int i = 0; i < arrlen; i++) {
            if(p_name == stdvars[i].name) {
                isvarfnd = true;
                varfndid = i;
            }
    }
        p_id = varfndid;

        return isvarfnd;
    }

    void setvar(std::string const& p_name, std::string const& p_value)
    {
        int varfndid = 0;
        bool isvarfnd = fetch_pvarsid(p_name, varfndid);

        if(isvarfnd) stdvars[varfndid].value = p_value;
        else perror_disp("stdvar not found", 0);
    }

    std::string getvar(std::string const& name)
    {
        int id = -1;
        bool isvarfnd = fetch_pvarsid(name, id);
        std::string rtr_val;

        if(isvarfnd) rtr_val = stdvars[id].value;
        else {
            std::string err_msg = "std var does not exist (" + name + ")";
            perror_disp(err_msg.c_str(), 1);
        }

        return rtr_val;
    }

    //Set the value of a standard program variable
    void setstdvars(std::string const& p_name, std::string const& p_value)
    {
        pvars::setvar(p_name, p_value);
    }

    //Copy the value of a standard program variable
    std::string getstdvars(std::string const& p_name)
    {
        return pvars::getvar(p_name);
    }
}
