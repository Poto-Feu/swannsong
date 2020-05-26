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
#include "find.hpp"
#include "room/room_io.h"
#include "interpreter/parser.hpp"
#include "stringsm.h"

namespace room_find
{
    /*Fetch the line where a Room property is present*/
    bool room_property(std::string& value, std::string const prop, int roomln)
    {
        bool propfound = false;
        bool is_end = false;

        ++roomln;

        for(int i = roomln; !propfound && !is_end; ++i)
        {
            std::string buf;
            std::string fw;

            bool is_eof = !roomio::fetch_ln(buf, i);

            if(is_eof)
            {
                perror_disp("find_room_property has hit EOF", true);
            } else
            {
                fw = stringsm::getfw(buf);

                if(fw == "CHOICES" || fw == "ATLAUNCH")
                {
                    i = parser::skip_until_end(i);
                } else if(fw == prop)
                {
                    buf.erase(0, fw.size());

                    while(buf[0] == ' ' || buf[0] == '\t') buf.erase(0, 1);

                    value = buf;
                    propfound = true;

                } else if(fw == "END") is_end = true;
            }
        }
        return propfound;
    }

    /*Fetch the line where a Choice declaration start ; the startln must point
    to the beginning of the Choices decleration set*/
    bool onechoiceline(int num, int startln, int& ln)
    {
        bool choicefound = false;
        bool is_end = false;
        int currln = startln + 1;

        if(num < 0 || num > 9) perror_disp("choice number not allowed", 1);

        for(int i = currln; !choicefound && !is_end; i++)
        {
            std::string buf;
            bool is_eof = !roomio::fetch_ln(buf, i);

            if(is_eof) perror_disp("find_onechoiceline has hit EOF", 1);
            if(buf[0] == 'c' && buf[1] == num + '0' && buf.size() == 2)
            {
                ln = i;
                choicefound = true;
            } else if(buf[0] == 'c' && buf[1] != num + '0' && buf.size() == 2)
            {
                i = parser::skip_until_end(i);
            } else if(buf  == "END") is_end = true;
            else  perror_disp("wrong input in find_onechoiceline", true);
        }
        return choicefound;
    }

    /*Fetch the line where a specific block is present beginning from a 
    specified line*/
    bool blockline(int& foundln, int p_ln, std::string ins)
    {
        bool is_fnd = false;
        bool is_end = false;
        bool in_choices = false;
        int currln = p_ln + 1;
        
        for(int i = currln; !is_fnd && !is_end; ++i)
        {
            std::string buf;
            bool is_eof = !roomio::fetch_ln(buf, i);
            
            if(is_eof) perror_disp("find_blockline has hit EOF", true);
            if(ins == buf && !in_choices)
            {
                is_fnd = true;
                foundln = i;
            } else if(buf == "ATLAUNCH")
            {
                if(in_choices)
                {
                    perror_disp("ATLAUNCH instruction inside CHOICES list",
                            true);
                }
                else i = parser::skip_until_end(i);
            } else if(buf == "CHOICES")
            {
                if(in_choices)
                {
                    perror_disp("CHOICES instruction inside CHOICES list",
                            true);
                } else in_choices = true;
            } else if(buf[0] == 'c' && isdigit(buf[1]) && buf.size() == 2)
            {
                if(in_choices) i = parser::skip_until_end(i);
                else perror_disp("CHOICE block outside CHOICES list", true);
            } else if(buf == "END")
            {
                if(in_choices) in_choices = false;
                else is_end = true;
            }

            buf.clear();
        }

        return is_fnd;
    }

    /*Return the line where the ATLAUNCH block start*/
    bool atlaunchline(int& foundln, int ln)
    {
        const std::string ins = "ATLAUNCH";
        return blockline(foundln, ln, ins);
    }

    /*Return the line where the CHOICES block start*/
    bool choicesline(int& foundln, int room_ln)
    {
        const std::string ins = "CHOICES";
        return blockline(foundln, room_ln, ins);
    }

    /*Fetch the beginning line of the room definition in file*/
    int roomline(std::string id)
    {
        int ln = 0;
        std::string roomline = "ROOM";

        roomline += ' ';
        roomline += id;

        if(!roomio::find_ind(ln, roomline))
        {
            std::string err_msg = "room not found in file (" + id + ")";

            perror_disp(err_msg.c_str(), true);
        }
        return ln;
    }

    bool roomline(int* r_ln, std::string id)
    {
        std::string roomline = "ROOM";

        roomline += ' ';
        roomline += id;

        if(!roomio::find_ind(*r_ln, roomline))
        {
            return false;
        } else return true;
    }
}
