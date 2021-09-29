/*
    Copyright (C) 2021 Adrien Saad

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
    along with SwannSong Adventure.  If not, see
    <https://www.gnu.org/licenses/>.
*/

#ifndef LOCAL_CONF_VARS
#define LOCAL_CONF_VARS

#include <memory>
#include <string>

// Header related to local configuration variables

namespace LocalConfVars {
    struct lcv_data;
    typedef std::shared_ptr<lcv_data> lcv_data_ptr;

    lcv_data_ptr init_data(std::string const& local_conf_path,
            bool reset_conf = false);
    bool change_value(lcv_data_ptr const& lcv, std::string const& id,
            std::string const& value);
    /* Same as change_value, except it will always set a value even if the
     * local configuration variable does not exists. */
    void set_value(lcv_data_ptr const& lcv, std::string const& id,
            std::string const& value);
    std::string const* get_value(lcv_data_ptr const& lcv,
            std::string const& id);
    bool write_to_file(lcv_data_ptr const& lcv,
            std::string const& local_conf_path);
}
#endif
