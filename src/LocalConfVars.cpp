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

#include <unordered_map>

extern "C" {
#include <jansson.h>
}

#include "LocalConfVars.hpp"
#include "fileio.hpp"
#include "game_error.hpp"

struct LocalConfVars::lcv_data {
    std::unordered_map<std::string, std::string> map;
};

static const std::string LOCAL_CONF_FILENAME = "conf_vars.txt";

static bool does_map_key_exists(
        std::unordered_map<std::string, std::string> const& map,
        std::string const& id)
{
    return map.find(id) != map.end();
}

static bool check_each_var_json(LocalConfVars::lcv_data_ptr const& lcv,
        json_t* lcv_json)
{
    json_t* var_json;
    const char* key;

    json_object_foreach(lcv_json, key, var_json) {
        json_error_t error;
        const char* var_val;

        if(json_unpack_ex(var_json, &error, 0, "s", &var_val) != 0) {
            game_error::fatal_error("Cannot get LocalConfVar value: "
                    + std::string(error.text) + " (line "
                    + std::to_string(error.line) + ")");
        } else {
            lcv->map[key] = var_val;
        }
    }

    return true;
}

static bool read_from_file(LocalConfVars::lcv_data_ptr const& lcv,
        std::string const& file_path)
{
    json_t* lcv_json;
    json_error_t error;

    lcv_json = json_load_file(file_path.c_str(), JSON_REJECT_DUPLICATES,
            &error);
    if(!lcv_json) {
        game_error::fatal_error("Cannot parse LocalConfVars json: "
                + std::string(error.text) + " (line "
                + std::to_string(error.line) + ")");
        return false;
    } else {
        check_each_var_json(lcv, lcv_json);
        json_decref(lcv_json);
        return true;
    }
}

std::shared_ptr<LocalConfVars::lcv_data> LocalConfVars::init_data(
        std::string const& local_conf_path, bool reset_conf)
{
    std::shared_ptr<lcv_data> lcv_ptr(new lcv_data);
    std::string file_path = local_conf_path + LOCAL_CONF_FILENAME;

    if(!reset_conf) {
        if(!read_from_file(lcv_ptr, file_path)) {
            return nullptr;
        }
    }

    return lcv_ptr;
}

std::string const* LocalConfVars::get_value(lcv_data_ptr const& lcv,
        std::string const& id)
{
    if(!does_map_key_exists(lcv->map, id)) {
        return nullptr;
    } else {
        return &lcv->map.at(id);
    }
}

void LocalConfVars::set_value(lcv_data_ptr const& lcv, std::string const& id,
        std::string const& value)
{
    lcv->map[id] = value;
}

static bool create_json_vars_strings(LocalConfVars::lcv_data_ptr const& lcv,
        json_t* root_json)
{
    for(auto const& it : lcv->map) {
        json_t* var_json;

        if(!(var_json = json_string(it.second.c_str()))) {
            game_error::emit_warning("Cannot create LocalConfVar JSON string");
            return false;
        }

        if(json_object_set_new(root_json, it.first.c_str(), var_json) == -1) {
            game_error::emit_warning(
                    "Cannot add LocalConfVar JSON string to root object");
            json_decref(var_json);

            return false;
        }
    }

    return true;
}

bool LocalConfVars::write_to_file(lcv_data_ptr const& lcv,
        std::string const& local_conf_path)
{
    const std::string file_path = local_conf_path + LOCAL_CONF_FILENAME;
    json_t* root_json;

    if(!(root_json = json_object())) {
        game_error::fatal_error("Cannot create conf_vars JSON root object");
        return false;
    } else if(!create_json_vars_strings(lcv, root_json)) {
        json_decref(root_json);
        return false;
    } if(json_dump_file(root_json, file_path.c_str(), JSON_INDENT(4)) != 0) {
        game_error::fatal_error("Cannot write savefile JSON to file");
        json_decref(root_json);
        return false;
    } else {
        json_decref(root_json);
        return true;
    }
}
