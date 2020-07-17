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

#include "fileio/save/SaveFileBuffer.hpp"

SaveFileBuffer::SaveFileBuffer() { }

void SaveFileBuffer::appendString(const char* p_value, uint32_t p_length)
{
    saveFileBufferVector temp_vec(p_value, p_value + p_length);
    buffer_content.insert(buffer_content.end(), temp_vec.begin(), temp_vec.end());
}

void SaveFileBuffer::appendString(std::string const& p_value)
{
    appendString(p_value.c_str(), p_value.length());
}

void SaveFileBuffer::appendSaveFileBufferVector(saveFileBufferVector const& p_value)
{
    buffer_content.insert(buffer_content.end(), p_value.begin(), p_value.end());
}

void SaveFileBuffer::truncateEnd(uint32_t p_length)
{
    buffer_content.resize(buffer_content.size() - p_length);
}

saveFileBufferVector SaveFileBuffer::getBufferVector()
{
    return buffer_content;
}

saveFileBufferVector const& SaveFileBuffer::getBufferVectorConstRef()
{
    return buffer_content;
}

saveFileBufferVector_type* SaveFileBuffer::getBufferVectorPointer()
{
    return &buffer_content[0];
}

uint32_t SaveFileBuffer::getBufferSize()
{
    return buffer_content.size();
}
