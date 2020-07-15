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

#ifndef SAVEFILE_BUFFER_HPP
#define SAVEFILE_BUFFER_HPP

#include <cstring>
#include <string>
#include <type_traits>
#include <vector>

typedef unsigned char saveFileBufferVector_type;
typedef std::vector<saveFileBufferVector_type> saveFileBufferVector;

class SaveFileBuffer
{
    public:

        SaveFileBuffer();

        //Various functions to append content to the vector
        template<typename T,
        typename = std::enable_if_t<std::is_integral<T>::value>>
        void appendNumber(T p_value)
        {
            const uint32_t T_type_size = sizeof(T);
            unsigned char temp_array[T_type_size];

            memcpy(temp_array, &p_value, sizeof(T));
            std::vector<unsigned char> temp_vec(temp_array, temp_array + T_type_size);
            buffer_content.insert(buffer_content.end(), temp_vec.begin(), temp_vec.end());
        }
        void appendString(const char* p_value, uint32_t p_length);
        void appendString(std::string const& p_value);
        void appendSaveFileBufferVector(saveFileBufferVector const& p_value);

        //Remove a specified number of characters starting from the end of the vector
        void truncateEnd(uint32_t p_length);

        //Return a copy of the buffer vector
        saveFileBufferVector getBufferVector();
        //Return a const reference to the buffer vector
        saveFileBufferVector const& getBufferVectorConstRef();
        //Return a pointer for the first item of the vector
        saveFileBufferVector_type* getBufferVectorPointer();

        uint32_t getBufferSize();

    private:

        saveFileBufferVector buffer_content;
};

#endif
