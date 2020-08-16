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

#include "fileio/save/save_file.hpp"
#include "fileio/save/SaveFile.hpp"
#include "files_path.hpp"
#include "pcurses.hpp"
#include "pstrings.h"

namespace save_file
{
    void start_saving(data_struct p_struct)
    {
        auto paths = files_path::getpaths();
        paths.local_data_path += "save/";

        SaveFile mainsave(paths.local_data_path, { "mainsave", p_struct.room_name,
                p_struct.player_data });
        if(mainsave.writeToFile()) {
            display_server::clear_screen();
            pcurses::display_center_string(pstrings::fetch("save_success"), pcurses::lines / 2);
            pcurses::display_penter_message();
        } else {
            display_server::clear_screen();
            pcurses::display_center_string(pstrings::fetch("save_failed"), pcurses::lines / 2);
            pcurses::display_penter_message();
        }
    }
}
