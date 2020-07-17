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

#ifndef SAVEFILE_HPP
#define SAVEFILE_HPP

#include <cstdint>
#include <fstream>
#include <filesystem>
#include "fileio/save/SaveChunk.hpp"
#include "fileio/save/SaveFileBuffer.hpp"

struct SaveFile_data
{
    std::string save_name;
    std::string room_name;
};

class SaveFile
{
    public:

        SaveFile(std::filesystem::path const& local_data_path, SaveFile_data const& p_data);

        template<typename T,
        typename = std::enable_if_t<std::is_integral<T>::value>>
        void appendNumber(T p_value)
        {
            file_buffer.appendNumber(p_value);
        }

        void appendString(std::string const&& p_value);
        void appendSaveFileBufferVector(saveFileBufferVector const& p_value);

        //Perform various operations for the savefile write
        bool writeToFile();

    private:

        std::filesystem::path file_path;
        SaveFileBuffer file_buffer;

        void add_header();
        void add_chunks(SaveFile_data const& p_data);

        //Write the content of the SaveFileBuffer to the savefile
        void output_to_file();

        //Check if the output file was correctly written
        bool check_save_integrity();

        void write_chunk(SaveChunk p_chunk);
};

#endif
