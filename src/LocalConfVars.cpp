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
    along with SwannSong Adventure.
    If not, see <https://www.gnu.org/licenses/>.
*/

#include <fstream>
#include <unordered_map>

#include "LocalConfVars.hpp"
#include "fileio/conf_files.hpp"
#include "fileio/fileio.h"
#include "game_error.hpp"
#include "stringsm.h"

struct LocalConfVars::lcv_data {
    std::unordered_map<std::string, std::string> map;
};

static const std::string LOCAL_CONF_FILENAME = "conf_vars.txt";

/* The corresponding value is the default value of the variable if it was not
 * defined in the conf file */
static const std::unordered_map<std::string, std::string> local_conf_vars({
        {"firstlaunch", "1"},
        {"lang", "eng"}
});

static bool does_map_key_exists(
        std::unordered_map<std::string, std::string> const& map,
        std::string const& id)
{
    return map.find(id) != map.end();
}

static void read_from_file(LocalConfVars::lcv_data_ptr data,
        std::string const& conf_file_path)
{
    std::vector<std::string> file_content = fileio::copy_to_vector(
            conf_file_path);

    for(auto& it : file_content) {
        std::string var;
        std::string value;

        stringsm::rtab(it);

        if(it == "" || it[0] == '#') {
            continue;
        }

        if(!conf_files::split_var(var, value, it)) {
            game_error::emit_warning("Wrong variable formatting: " + it);
            continue;
        }
        data->map[var] = value;
    }
}

std::shared_ptr<LocalConfVars::lcv_data> LocalConfVars::init_data(
        std::string const& local_conf_path, bool reset_conf)
{
    std::shared_ptr<lcv_data> lcv_ptr(new lcv_data);
    std::string conf_file_path = local_conf_path + LOCAL_CONF_FILENAME;

    if(fileio::file_exists(conf_file_path) && !reset_conf) {
        read_from_file(lcv_ptr, conf_file_path);
    } else {
        lcv_ptr->map = local_conf_vars;
        write_to_file(lcv_ptr, local_conf_path);
    }

    return lcv_ptr;
}

bool LocalConfVars::change_value(lcv_data_ptr const data,
        std::string const& id,
        std::string const& value)
{
    if(!does_map_key_exists(data->map, id)) {
        return false;
    } else {
        data->map[id] = value;
        return true;
    }
}

std::string const* LocalConfVars::get_value(lcv_data_ptr data,
        std::string const& id)
{
    if(!does_map_key_exists(data->map, id)) {
        return nullptr;
    } else {
        return &data->map.at(id);
    }
}

bool LocalConfVars::write_to_file(lcv_data_ptr data,
        std::string const& local_conf_path)
{
    std::ofstream file_stream(local_conf_path + LOCAL_CONF_FILENAME);

    for(auto const& it : data->map) {
        file_stream << it.first << "=" << "\"" << it.second << "\""
            << std::endl;
    }
    return true;
}
