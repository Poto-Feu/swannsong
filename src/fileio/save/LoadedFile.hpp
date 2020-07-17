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

#ifndef LOADEDFILE_HPP
#define LOADEDFILE_HPP

#include <filesystem>
#include <string>
#include <vector>
#include "fileio/save/SaveFileBuffer.hpp"
#include "fileio/save/save_const.hpp"
#include "vars/gvars.hpp"
#include "inventory.hpp"

struct LoadedFile_data
{
    save_const::version_type FILE_MAJ_VER;
    save_const::version_type FILE_MIN_VER;

    bool file_exists = true;
    bool is_savefile = true;
    bool is_corrupted = false;
    std::string room;
    gvars::gvarVector gvar_vector;
    inventory::gitemVector gitem_vector;

};

class LoadedFile
{
    public:

        LoadedFile(std::filesystem::path const& local_data_path, std::string const& save_name);

        LoadedFile_data getLoadedFile_data();

    private:

        //The metadata of a chunk
        struct chunk_meta
        {
            save_const::chunk_type type;
            uint32_t length;
        };

        SaveFileBuffer file_buffer;

        std::filesystem::path file_path;

        //Store the "byte position" where the file should be read
        uint32_t read_pos = 0;
        LoadedFile_data obj_data;

        void read_file();

        //Compare the characters of a string with the bytes of the file
        bool compare_string(std::string p_str, uint32_t p_size);

        //Extract the checksum from the end of the file and compare it to the actual file content
        bool verify_checksum();
        //Check if the first bytes of the files match those of the game savefile format
        bool check_signature();
        void get_format_version();

        chunk_meta get_chunk_meta();
        bool interpret_chunk();

        //Interpret the content of a CURROOM chunk
        void CURROOM_chunk(uint32_t chk_length);
        //Interpret the content of a PVARIAB chunk
        bool PVARIAB_chunk();
        //Interpret the content of an INVITEM chunk
        bool INVITEM_chunk();
};

#endif
