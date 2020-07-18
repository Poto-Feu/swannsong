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

#include <openssl/sha.h>
#include "crypt_functions.hpp"

namespace crypt_functions
{
    SHA1Checksum getSHA1Checksum(std::vector<unsigned char> p_char_vec)
    {
        unsigned char hash_array[SHA_DIGEST_LENGTH];
        SHA1(p_char_vec.data(), p_char_vec.size(), hash_array);
        SHA1Checksum rtrn_value(hash_array, hash_array + SHA_DIGEST_LENGTH);

        return rtrn_value;
    }
}
