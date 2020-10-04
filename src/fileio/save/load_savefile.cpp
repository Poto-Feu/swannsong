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

#include "fileio/save/load_savefile.hpp"
#include "files_path.hpp"
#include "game_error.hpp"
#include "pcurses.hpp"

namespace load_savefile
{
    LoadedFile_data start_loading(PStrings const& program_strings)
    {
        std::filesystem::path local_data_path = files_path::get_local_data_path();
        local_data_path += "save/";

        LoadedFile loaded_savefile(local_data_path, "mainsave");
        auto savefile_data = loaded_savefile.getLoadedFile_data();

        if(!savefile_data.file_exists || !savefile_data.is_savefile) {
            display_server::clear_screen();
            pcurses::display_center_string(program_strings.fetch("load_nofile"),
                    pcurses::lines / 2);
            pcurses::display_penter_message(program_strings);
        } else if(savefile_data.is_corrupted) {
            display_server::clear_screen();
            pcurses::display_center_string(program_strings.fetch("load_corrupted"),
                    pcurses::lines / 2);
            pcurses::display_penter_message(program_strings);
        }

        return savefile_data;
    }
}
