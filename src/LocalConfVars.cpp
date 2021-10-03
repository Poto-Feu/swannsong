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
#include "fileio/fileio.hpp"
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
    const char* key;
    json_t* var_json;

    json_object_foreach(lcv_json, key, var_json) {
        if(json_typeof(var_json) != JSON_STRING) {
            game_error::emit_warning("Non-string value in local conf file");
        } else {
            const char* var_str = json_string_value(var_json);

            if(!var_str) {
                game_error::fatal_error("Cannot get value from JSON string");

                return false;
            } else {
                lcv->map[key] = var_str;
            }
        }
    }

    return true;
}

static bool parse_lcv_json(LocalConfVars::lcv_data_ptr const& lcv,
        std::string const& file_content)
{
    json_t* lcv_json;

    lcv_json = json_loads(file_content.c_str(), JSON_REJECT_DUPLICATES, NULL);
    if(!lcv_json) {
        game_error::emit_warning("Cannot parse LocalConfVars json");
        return false;
    } else {
        check_each_var_json(lcv, lcv_json);
        json_decref(lcv_json);
        return true;
    }
}

static bool read_from_file(LocalConfVars::lcv_data_ptr const& lcv,
        std::string const& conf_file_path)
{
    std::string file_content;

    if(!fileio::copy_to_string(conf_file_path, file_content)) {
        return false;
    } else {
        return parse_lcv_json(lcv, file_content);
    }
}

std::shared_ptr<LocalConfVars::lcv_data> LocalConfVars::init_data(
        std::string const& local_conf_path, bool reset_conf)
{
    std::shared_ptr<lcv_data> lcv_ptr(new lcv_data);
    std::string conf_file_path = local_conf_path + LOCAL_CONF_FILENAME;

    if(!reset_conf) {
        read_from_file(lcv_ptr, conf_file_path);
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
    json_t* root_json;
    char* buf_c;
    std::string buf;

    if(!(root_json = json_object())) {
        game_error::fatal_error("Cannot create conf_vars JSON root object");
        return false;
    } else if(!create_json_vars_strings(lcv, root_json)) {
        json_decref(root_json);
        return false;
    } else if(!(buf_c = json_dumps(root_json, JSON_INDENT(4)))) {
        game_error::fatal_error("Cannot dump LocalConfVars JSON root object");
        return false;
    }

    buf = buf_c;
    buf += '\n';
    free(buf_c);
    json_decref(root_json);

    return fileio::write_to_file(local_conf_path + LOCAL_CONF_FILENAME, buf);
}
