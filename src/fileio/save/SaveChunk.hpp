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

#ifndef SAVE_CHUNK_HPP
#define SAVE_CHUNK_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include "fileio/save/SaveFileBuffer.hpp"
#include "fileio/save/save_const.hpp"

struct saveChunkData
{
    save_const::chunk_type_type chunk_type;
    uint32_t chunk_size;
    saveFileBufferVector buffer_vector;
};

class SaveChunk
{
    public:
        SaveChunk(save_const::chunk_type p_name, std::string const& p_string);
        SaveChunk(save_const::chunk_type p_name, std::vector<saveFileBufferVector> p_chk_vec);
        SaveChunk(save_const::chunk_type p_name, saveFileBufferVector const& p_chk_vec);

        template<typename T,
        typename = typename std::enable_if_t<std::is_fundamental_v<T>>>
        SaveChunk(save_const::chunk_type p_id, T p_value) : id(p_id)
        {
            copy_bytes_to_array(chunk_content, &p_value, 1);
        }

        template<typename T,
        typename = typename std::enable_if_t<std::is_fundamental_v<std::remove_pointer_t<T>>
            && std::is_pointer_v<T>
            >>
        SaveChunk(save_const::chunk_type p_id, T p_pointer, uint32_t p_size) : id(p_id)
        {
            copy_bytes_to_array(chunk_content, p_pointer, p_size);
        }

        saveChunkData getSaveChunkData() const;
        saveFileBufferVector getChunkAsVector();
        uint32_t getVectorSize();

    private:

        save_const::chunk_type id;
        saveFileBufferVector chunk_content;

        //Copy the raw bytes of the content of a fundamental type pointer to chunk_content
        template<typename T,
        typename = typename std::enable_if_t<std::is_fundamental_v<std::remove_pointer_t<T>>
            && std::is_pointer_v<T>
            >>
        void copy_bytes_to_array(saveFileBufferVector& p_vec, T p_pointer, uint32_t p_size)
        {
            p_size *= sizeof(*p_pointer);

            saveFileBufferVector temp_vec(p_pointer, p_pointer + p_size);
            p_vec.insert(p_vec.end(), temp_vec.begin(), temp_vec.end());
        }

        //Copy the raw bytes of a fundamental type pointer to chunk_content
        template<typename T,
        typename = typename std::enable_if_t<std::is_fundamental_v<T>
            && !std::is_pointer_v<T>
            >>
        void copy_fundamental_type_to_array(saveFileBufferVector& p_vec, T p_val)
        {
            uint32_t p_size = sizeof(p_val);

            unsigned char* raw_bytes = new unsigned char[p_size];
            std::memcpy(raw_bytes, &p_val, p_size);

            saveFileBufferVector temp_vec(raw_bytes, raw_bytes + p_size);
            p_vec.insert(p_vec.end(), temp_vec.begin(), temp_vec.end());

            delete[] raw_bytes;
        }
};

#endif
