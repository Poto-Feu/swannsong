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

#include "fileio/save/SaveChunk.hpp"

SaveChunk::SaveChunk(save_const::chunk_type p_id, std::string const& p_string) :
    SaveChunk(p_id, (unsigned char*)p_string.c_str(), p_string.size()) { }

SaveChunk::SaveChunk(save_const::chunk_type p_id, saveFileBufferVector const& p_chk_vec) :
    SaveChunk(p_id, p_chk_vec.data(), p_chk_vec.size()) { }

SaveChunk::SaveChunk(save_const::chunk_type p_id, std::vector<saveFileBufferVector> p_chk_vec) :
    id(p_id)
{
    for(auto const& it : p_chk_vec) {
        copy_saveFileBufferVector_to_array(chunk_content, it);
    }
}

saveFileBufferVector SaveChunk::getChunkAsVector()
{
    uint32_t chunk_content_size = chunk_content.size();
    saveFileBufferVector raw_chunk;

    save_const::chunk_type_type id_int = static_cast<save_const::chunk_type_type>(id);
    copy_fundamental_type_to_array(raw_chunk, id_int);
    copy_fundamental_type_to_array(raw_chunk, chunk_content_size);
    copy_saveFileBufferVector_to_array(raw_chunk, chunk_content);

    return raw_chunk;
}

void SaveChunk::copy_saveFileBufferVector_to_array(saveFileBufferVector& p_vec,
                saveFileBufferVector const& new_vec)
{
    p_vec.insert(p_vec.end(), new_vec.begin(), new_vec.end());
}
