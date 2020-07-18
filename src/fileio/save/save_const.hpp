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

#ifndef SAVE_CONST_HPP
#define SAVE_CONST_HPP

#include <cstdint>

namespace save_const
{
    typedef uint16_t chunk_type_type;
    typedef uint16_t version_type;

    const uint32_t SIGNATURE_size = 4;
    const char* const FILE_SIGNATURE = "SWSA";
    const char* const SAVE_FILE_SIGNATURE = "SAVE";

    //Savefile format version
    const uint16_t MAJOR_VERSION = 1;
    const uint16_t MINOR_VERSION = 0;

    enum class chunk_type : chunk_type_type
    {
        GAMNAME = 0x00,
        SAVNAME = 0x01,

        CURROOM = 0x02,

        PVARIAB = 0x03,
        PVARNAM = 0x04,
        PVARVAL = 0x05,

        INVITEM = 0x06,
        ITEMNAM = 0x07,
        ITEMVAL = 0x08
    };
}

#endif
