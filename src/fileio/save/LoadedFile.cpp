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

#include <cstring>
#include <fstream>
#include <iterator>
#include <memory>

#include "fileio/save/LoadedFile.hpp"
#include "crypt_functions.hpp"
#include "game_error.hpp"

LoadedFile::LoadedFile(std::filesystem::path const& save_folder_path, std::string const& save_name)
    : file_path(save_folder_path)
{
    file_path += save_name;
    file_path += ".save";

    read_file();
    if(obj_data.file_exists) {
        if(!verify_checksum()) obj_data.is_corrupted = true;
    }
}

void LoadedFile::read_file()
{
    std::ifstream savefile_input(file_path, std::ios::binary);

    if(savefile_input.is_open()) {
        saveFileBufferVector temp_buf((std::istreambuf_iterator<char>(savefile_input)),
                std::istreambuf_iterator<char>());

        file_buffer.appendSaveFileBufferVector(temp_buf);
    } else obj_data.file_exists = false;
}

LoadedFile_data LoadedFile::getLoadedFile_data()
{
    if(obj_data.is_corrupted || !obj_data.file_exists) return std::move(obj_data);
    else if(!check_signature()) {
        obj_data.is_savefile = false;
        return std::move(obj_data);
    }

    get_format_version();

    while(read_pos < file_buffer.getBufferSize()) {
        if(!interpret_chunk()) {
            obj_data.is_corrupted = true;
            break;
        }
    }
    return std::move(obj_data);
}

bool LoadedFile::compare_string(std::string p_str, uint32_t p_size)
{
    if(read_pos + p_size > file_buffer.getBufferSize()) return false;

    if(!std::strncmp(p_str.c_str(), (char*)file_buffer.getBufferVectorPointer() + read_pos, p_size))
    {
        read_pos += p_size;
        return true;
    } else return false;
}

bool LoadedFile::verify_checksum()
{
    using namespace crypt_functions;

    auto buffer_ptr = file_buffer.getBufferVectorPointer();
    auto buffer_size = file_buffer.getBufferSize();

    saveFileBufferVector extracted_checksum(buffer_ptr + buffer_size - SHA1_size,
            buffer_ptr + buffer_size);
    file_buffer.truncateEnd(SHA1_size);
    auto file_checksum = getSHA1Checksum(file_buffer.getBufferVectorConstRef());
    
    if(file_checksum == extracted_checksum) return true;
    else return false;
}

bool LoadedFile::check_signature()
{
    return compare_string(save_const::FILE_SIGNATURE, save_const::SIGNATURE_size)
        && compare_string(save_const::SAVE_FILE_SIGNATURE, save_const::SIGNATURE_size);
}

void LoadedFile::get_format_version()
{
    using namespace save_const;

    saveFileBufferVector_type* buffer_ptr = file_buffer.getBufferVectorPointer();

    std::memcpy(&obj_data.FILE_MAJ_VER, buffer_ptr + read_pos, sizeof(version_type));
    read_pos += sizeof(version_type);
    std::memcpy(&obj_data.FILE_MIN_VER, buffer_ptr + read_pos, sizeof(version_type));
    read_pos += sizeof(version_type);
}

LoadedFile::chunk_meta LoadedFile::get_chunk_meta()
{
    LoadedFile::chunk_meta chk_meta;

    saveFileBufferVector_type* buffer_ptr = file_buffer.getBufferVectorPointer();
    std::memcpy(&chk_meta.type, buffer_ptr + read_pos, sizeof(chk_meta.type));
    read_pos += sizeof(chk_meta.type);
    std::memcpy(&chk_meta.length, buffer_ptr + read_pos, sizeof(chk_meta.length));
    read_pos += sizeof(chk_meta.length);

    return chk_meta;
}

bool LoadedFile::interpret_chunk()
{
    using namespace save_const;

    auto chk_meta = get_chunk_meta();

    switch(chk_meta.type) {
        case chunk_type::GAMNAME:
            read_pos += chk_meta.length;
            break;
        case chunk_type::CURROOM:
            CURROOM_chunk(chk_meta.length);
            break;
        case chunk_type::INVITEM:
            if(!INVITEM_chunk()) return false;
            break;
        case chunk_type::PVARIAB:
            if(!PVARIAB_chunk()) return false;
            break;
        default:
            game_error::emit_warning("Savefile chunk ignore ("
                    + std::to_string((chunk_type_type)chk_meta.type) + ")");
            read_pos += chk_meta.length;
            break;
    }

    return true;
}

void LoadedFile::CURROOM_chunk(uint32_t chk_length)
{
    saveFileBufferVector_type* buffer_ptr = file_buffer.getBufferVectorPointer();

    unsigned char* room_char = new unsigned char[chk_length + 1];
    room_char[chk_length] = '\0';
    std::memcpy(room_char, buffer_ptr + read_pos, chk_length);

    std::string room_str((char*)room_char);
    obj_data.room = std::move(room_str);
    delete[] room_char;

    read_pos += chk_length;
}

bool LoadedFile::INVITEM_chunk()
{
    using namespace save_const;

    saveFileBufferVector_type* buffer_ptr = file_buffer.getBufferVectorPointer();

    bool ITEMNAM_done = false;
    bool ITEMVAL_done = false;
    inventory::gitem new_gitem;

    for(unsigned int i = 0; i < 2; ++i) {
        auto chk_meta = get_chunk_meta();

        switch(chk_meta.type) {
            case chunk_type::ITEMNAM:
                if(!ITEMNAM_done) {
                    char* raw_gitem_name = new char[chk_meta.length+ 1];
                    raw_gitem_name[chk_meta.length] = '\0';

                    ITEMNAM_done = true;
                    std::memcpy(raw_gitem_name, buffer_ptr + read_pos, chk_meta.length);
                    new_gitem.name = std::string(raw_gitem_name);
                    delete[] raw_gitem_name;

                    read_pos += chk_meta.length;
                } else {
                    game_error::emit_warning("ITEMNAM chunk present twice in INVITEM chunk");
                    return false;
                }
                break;
                
            case chunk_type::ITEMVAL:
                if(!ITEMVAL_done) {
                    if(chk_meta.length == sizeof(gvar_type)) {
                        ITEMVAL_done = true;
                        std::memcpy(&new_gitem.val, buffer_ptr + read_pos, chk_meta.length);

                        read_pos += chk_meta.length;
                    } else return false;
                } else {
                    game_error::emit_warning("ITEMVAL chunk present twice in INVITEM chunk");
                    return false;
                }
                break;

            default:
                game_error::emit_warning("Unknown chunk in INVITEM chunk");
                return false;
                break;
        }
    }

    if(ITEMNAM_done && ITEMVAL_done) {
        obj_data.inv.push_back(new_gitem);
        return true;
    } else return false;
}

bool LoadedFile::PVARIAB_chunk()
{
    using namespace save_const;

    saveFileBufferVector_type* buffer_ptr = file_buffer.getBufferVectorPointer();

    bool PVARNAM_done = false;
    bool PVARVAL_done = false;
    gvar new_gvar;

    for(unsigned int i = 0; i < 2; ++i) {
        auto chk_meta = get_chunk_meta();

        switch(chk_meta.type) {
            case chunk_type::PVARNAM:
                if(!PVARNAM_done) {
                    char* raw_gvar_name = new char[chk_meta.length + 1];
                    raw_gvar_name[chk_meta.length] = '\0';

                    PVARNAM_done = true;
                    std::memcpy(raw_gvar_name, buffer_ptr + read_pos, chk_meta.length);
                    new_gvar.name = std::string(raw_gvar_name);
                    delete[] raw_gvar_name;

                    read_pos += chk_meta.length;
                } else {
                    game_error::emit_warning("PVARNAM chunk present twice in PVARVAL chunk");
                    return false;
                }
                break;
                
            case chunk_type::PVARVAL:
                if(!PVARVAL_done) {
                    if(chk_meta.length == sizeof(gvar_type)) {
                        PVARVAL_done = true;
                        std::memcpy(&new_gvar.val, buffer_ptr + read_pos, chk_meta.length);

                        read_pos += chk_meta.length;
                    } else return false;
                } else {
                    game_error::emit_warning("PVARVAL chunk present twice in PVARVAL chunk");
                    return false;
                }
                break;

            default:
                game_error::emit_warning("Unknown chunk in PVARIAB chunk");
                return false;
                break;
        }
    }

    if(PVARNAM_done && PVARVAL_done) {
        obj_data.gvar_vector.push_back(new_gvar);
        return true;
    } else return false;
}
