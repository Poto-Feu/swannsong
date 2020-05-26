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
    const std::string pvars_undef = "undefined";

    struct pvar_struct
    {
        std::string name;
        std::string value;
    };

    std::vector<pvar_struct> stdvars =
    {
        {"lang", pvars_undef},
        {"roomfile", pvars_undef},
        {"csfile", pvars_undef}
    };

    std::vector<pvar_struct> gcvars =
    {
        {"langdir", pvars_undef},
        {"roomfile", pvars_undef},
        {"csfile", pvars_undef},
        {"defaultlang", pvars_undef},
        {"firstroom", pvars_undef}
    };

    bool fetch_pvarsid(std::string const p_name, int& p_id, bool isgcvar)
    {
        bool isvarfnd = false;
        int varfndid = -1;
        int arrlen = 0;

        if(isgcvar) arrlen = gcvars.size();
        else arrlen = stdvars.size();

        for(int i = 0; i < arrlen; i++)
        {
            if(isgcvar)
            {
                if(p_name == gcvars[i].name)
                {
                    isvarfnd = true;
                    varfndid = i;
                }
            } else
            {
                if(p_name == stdvars[i].name)
                {
                    isvarfnd = true;
                    varfndid = i;
                }
            }
        }
        p_id = varfndid;

        return isvarfnd;
    }

    void set(std::string const p_name, std::string const p_value, bool isgcvar)
    {
        int varfndid = 0;
        bool isvarfnd = fetch_pvarsid(p_name, varfndid, isgcvar);

        if(isvarfnd)
        {
            if(isgcvar)
            {
                gcvars[varfndid].value = p_value;
            } else
            {
                stdvars[varfndid].value = p_value;
            }
            
        } else
        {
            if(isgcvar) perror_disp("gcvar not found", 0);
            else perror_disp("stdvar not found", 0);
        }
    }

    std::string get(std::string const name, bool isgcvar)
    {
        int id = -1;
        bool isvarfnd = fetch_pvarsid(name, id, isgcvar);
        std::string rtr_val;

        if(isvarfnd)
        {
            if(isgcvar) rtr_val = gcvars[id].value;
            else rtr_val = stdvars[id].value;

        } else
        {
            if(isgcvar) perror_disp("gameconf var does not exist", 1);
            else perror_disp("std var does not exist", 1);
        }

        return rtr_val;
    }

    /*Set the value of a standard program variable*/
    void setstdvars(std::string const p_name, std::string const p_value)
    {
        pvars::set(p_name, p_value, false);
    }

    /*Set the value of a gameconf-defined variable*/
    void setgcvars(std::string const p_name, std::string const p_value)
    {
        pvars::set(p_name, p_value, true);
    }

    /*Copy the value of a standard program variable*/
    std::string getstdvars(std::string const p_name)
    {
        return pvars::get(p_name, false);
    }

    /*Copy the value of a gameconf-defined variable*/
    std::string getgcvars(std::string const p_name)
    {
        return pvars::get(p_name, true);
    }
}
