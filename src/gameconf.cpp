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

#include <algorithm>

extern "C" {
#include <jansson.h>
}

#include "gameconf.hpp"
#include "game_error.hpp"

struct gc_var {
    std::string name;
    std::vector<std::string> values;
};

struct gameconf::gcvars {
    std::vector<gc_var> vars_vec;
};

static bool parse_gameconf_json_array(gameconf::gcvars_ptr const& vars,
        std::string const& var_key, json_t* array_json)
{
    json_t* value_json;
    json_error_t error;
    std::vector<std::string> values;
    size_t i;

    values.reserve(json_array_size(array_json));

    json_array_foreach(array_json, i, value_json) {
        const char* var_val;

        if(json_unpack_ex(value_json, &error, 0, "s", &var_val) != 0) {
            game_error::fatal_error("Cannot get gameconf variable value: "
                    + std::string(error.text) + " (line "
                    + std::to_string(error.line) + ")");
            return false;
        } else {
            values.push_back(json_string_value(value_json));
        }
    }

    vars->vars_vec.push_back({ var_key, std::move(values) });

    return true;
}

static bool parse_gameconf_json(gameconf::gcvars_ptr const& vars,
        json_t* gameconf_json)
{
    const char* var_key;
    json_t* var_json;

    if(json_typeof(gameconf_json) != JSON_OBJECT) {
        game_error::fatal_error("gameconf root JSON is not an object");
        return false;
    }

    vars->vars_vec.reserve(json_object_size(gameconf_json));

    json_object_foreach(gameconf_json, var_key, var_json) {
        if(json_typeof(var_json) == JSON_STRING) {
            vars->vars_vec.push_back({
                    var_key, { json_string_value(var_json) }});
        } else if(json_typeof(var_json) == JSON_ARRAY) {
            /* No need to check the return value, "crash" on runtime if
             * needed. */
            parse_gameconf_json_array(vars, var_key, var_json);
        } else {
            game_error::emit_warning("gameconf JSON var is neither a STRING "
                    "nor an array");
        }
    }

    json_decref(gameconf_json);

    return true;
}

gameconf::gcvars_ptr gameconf::readfile(std::string const& data_path)
{
    const std::string file_path = data_path + "gameconf.json";
    json_t* gameconf_json;
    json_error_t error;
    gameconf::gcvars_ptr vars(new gameconf::gcvars);

    gameconf_json = json_load_file(file_path.c_str(), JSON_REJECT_DUPLICATES,
            &error);
    if(!gameconf_json) {
        game_error::fatal_error("Cannot parse gameconf json file: "
                + std::string(error.text) + " (line "
                + std::to_string(error.line) + ")");
        return nullptr;
    } else if(!parse_gameconf_json(vars, gameconf_json)) {
        json_decref(gameconf_json);
        return nullptr;
    }

    return vars;
}

static auto get_gameconf_var_iterator(std::vector<gc_var> const& vars_vec,
        std::string const& var_name)
{
    return std::find_if(vars_vec.cbegin(), vars_vec.cend(),
            [&var_name](struct gc_var const& var) {
            return var.name == var_name;
            });
}

std::vector<std::string> const* gameconf::get_var(
        gameconf::gcvars_ptr const& vars, std::string const& var_name)
{
    auto var_iterator = get_gameconf_var_iterator(vars->vars_vec, var_name);

    if(var_iterator == vars->vars_vec.cend()) {
        return nullptr;
    } else {
        return &var_iterator->values;
    }
}
