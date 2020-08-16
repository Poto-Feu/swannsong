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

#include "fileio/save/SaveFile.hpp"
#include "vars/gvars.hpp"
#include "crypt_functions.hpp"
#include "display_server.hpp"
#include "files_path.hpp"
#include "game_error.hpp"
#include "player/inventory.hpp"

using namespace save_const;

SaveFile::SaveFile(std::filesystem::path const& local_data_path, SaveFile_data const& p_data) :
    file_path(local_data_path)
{
    files_path::create_directory(file_path);
    file_path += p_data.save_name;
    file_path += ".save";

    add_header();
    add_chunks(p_data);

    auto file_checksum = crypt_functions::getSHA1Checksum(file_buffer.getBufferVector());
    file_buffer.appendSaveFileBufferVector(file_checksum);
}

bool SaveFile::writeToFile()
{
    output_to_file();

    if(check_save_integrity()) return true;
    else return false;
}

void SaveFile::appendString(std::string const&& p_value)
{
    file_buffer.appendString(p_value);
}

void SaveFile::appendSaveFileBufferVector(saveFileBufferVector const& p_value)
{
    file_buffer.appendSaveFileBufferVector(p_value);
}

//Private member functions
void SaveFile::add_header()
{
    file_buffer.appendString(FILE_SIGNATURE, SIGNATURE_size);
    file_buffer.appendString(SAVE_FILE_SIGNATURE, SIGNATURE_size);
    file_buffer.appendNumber(save_const::MAJOR_VERSION);
    file_buffer.appendNumber(save_const::MINOR_VERSION);
}

static void add_inventory_chunks(inventory::Inventory const& p_inv,
        std::vector<SaveChunk>& chunks_vector)
{
    for(auto const& it : p_inv) {
        SaveChunk itemnam_chk(chunk_type::ITEMNAM, it.name);
        SaveChunk itemval_chk(chunk_type::ITEMVAL, it.val);
        auto itemnam_chk_vec = itemnam_chk.getChunkAsVector();
        auto itemval_chk_vec = itemval_chk.getChunkAsVector();

        chunks_vector.push_back(
                SaveChunk(chunk_type::INVITEM,
                    std::vector<saveFileBufferVector>{ itemnam_chk_vec, itemval_chk_vec })
                );
    }
}

static void add_gvars_chunks(std::vector<SaveChunk>& chunks_vector)
{
    auto gvars_vec = gvars::get_gvars_vector();

    for(auto const& it : gvars_vec) {
        SaveChunk pvarnam_chk(chunk_type::PVARNAM, it.name);
        SaveChunk pvarval_chk(chunk_type::PVARVAL, it.val);
        auto pvarnam_chk_vec = pvarnam_chk.getChunkAsVector();
        auto pvarval_chk_vec = pvarval_chk.getChunkAsVector();

        chunks_vector.push_back(
                SaveChunk(chunk_type::PVARIAB,
                    std::vector<saveFileBufferVector>{ pvarnam_chk_vec, pvarval_chk_vec })
                );
    }
}

void SaveFile::add_chunks(SaveFile_data const& p_data)
{
    std::vector<SaveChunk> chunks_vector {
        SaveChunk(chunk_type::GAMNAME, "SwannSong Adventure"),
        SaveChunk(chunk_type::CURROOM, p_data.room_name)
    };

    add_inventory_chunks(p_data.player_data.inv, chunks_vector);
    add_gvars_chunks(chunks_vector);

    for(auto& it : chunks_vector) write_chunk(it);
}

void SaveFile::output_to_file()
{
    std::ofstream save_output_stream(file_path, std::ios::binary);
    save_output_stream.exceptions(std::ios::badbit | std::ios::failbit);

    save_output_stream.write(reinterpret_cast<char*>(file_buffer.getBufferVector().data()),
            file_buffer.getBufferSize());
}

bool SaveFile::check_save_integrity()
{
    uint32_t buffer_size = file_buffer.getBufferSize();
    std::ifstream save_input(file_path, std::ios::binary);
    char* read_buf = new char[buffer_size];

    save_input.read(read_buf, file_buffer.getBufferSize());

    if(!strncmp(read_buf, (const char*)file_buffer.getBufferVector().data(), buffer_size)) {
        delete[] read_buf;
        return true;
    } else {
        delete[] read_buf;
        return false;
    }
}

void SaveFile::write_chunk(SaveChunk p_chunk)
{
    appendSaveFileBufferVector(p_chunk.getChunkAsVector());
}
