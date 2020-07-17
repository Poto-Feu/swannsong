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
    uint32_t p_chk_vec_size = p_chk_vec.size();

    for(uint32_t i = 1; i < p_chk_vec_size; ++i) {
        p_chk_vec[0].insert(p_chk_vec[0].end(), p_chk_vec[i].begin(), p_chk_vec[i].end());
    }

    copy_bytes_to_array(chunk_content, p_chk_vec[0].data(), p_chk_vec[0].size());
}

saveFileBufferVector SaveChunk::getChunkAsVector()
{
    uint32_t chunk_content_size = chunk_content.size();
    saveFileBufferVector raw_chunk;

    save_const::chunk_type_type id_int = static_cast<save_const::chunk_type_type>(id);
    copy_fundamental_type_to_array(raw_chunk, id_int);
    copy_fundamental_type_to_array(raw_chunk, chunk_content_size);
    copy_bytes_to_array(raw_chunk, chunk_content.data(), chunk_content_size);

    return raw_chunk;
}
