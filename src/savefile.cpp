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

extern "C" {
#include <jansson.h>
}

#include "savefile.hpp"
#include "fileio.hpp"
#include "game_error.hpp"
#include "player/Player.hpp"

static const std::string game_name = "SwannSong Adventure";

static bool add_to_json_object(json_t* parent, std::string const& key,
        json_t* child)
{
    if(json_object_set_new(parent, key.c_str(), child) == -1) {
        game_error::fatal_error("Cannot add child to JSON object");
        return false;
    }

    return true;
}

static bool add_pair_to_json_object(json_t* object_json,
        std::string const& key, std::string const& value)
{
    json_t* child = json_string(value.c_str());

    if(!child) {
        game_error::fatal_error("Cannot create JSON string");
        json_decref(child);

        return false;
    }

    return add_to_json_object(object_json, key, child);
}

static bool set_game_vars_json_array(gvarVector const& gvars,
        json_t* root_json)
{
    json_t* gvars_array_json;

    gvars_array_json = json_array();
    if(!gvars_array_json) {
        game_error::fatal_error("Cannot create savefile game vars array");
        return false;
    }

    for(auto const& it : gvars) {
        json_t* gvar_json;
        json_error_t error;

        gvar_json = json_pack_ex(&error, 0, "{ s:s, s:i }", "id",
                it.name.c_str(), "value", it.val);
        if(!gvar_json) {
            game_error::fatal_error("Cannot create savefile JSON game var: "
                    + std::string(error.text) + " (line "
                    + std::to_string(error.line) + ")");
        } else if(json_array_append_new(gvars_array_json, gvar_json) == -1) {
            game_error::fatal_error("Cannot add game var to savefile array");
            json_decref(gvar_json);
            json_decref(gvars_array_json);
            return false;
        }
    }

    if(!add_to_json_object(root_json, "game_vars", gvars_array_json)) {
        json_decref(gvars_array_json);
        return false;
    }

    return true;
}

static bool set_items_json_array(
        std::vector<inventory::gitem> const& items, json_t* root_json)
{
    json_t* items_array_json;

    items_array_json = json_array();
    if(!items_array_json) {
        game_error::fatal_error("Cannot create savefile items array");
        return false;
    }

    for(auto const& it : items) {
        json_t* item_json;
        json_error_t error;

        item_json = json_pack_ex(&error, 0, "{ s:s, s:i }", "id",
                it.name.c_str(), "number", it.val);
        if(!item_json) {
            game_error::fatal_error("Cannot create savefile JSON inventory "
                    "item: " + std::string(error.text) + " (line "
                    + std::to_string(error.line) + ")");
        } else if(json_array_append_new(items_array_json, item_json) == -1) {
            game_error::fatal_error("Cannot add item to savefile array");
            json_decref(item_json);
            json_decref(items_array_json);
            return false;
        }
    }

    if(!add_to_json_object(root_json, "items", items_array_json)) {
        json_decref(items_array_json);
        return false;
    }

    return true;
}

static bool set_json_structure(Player const& player,
        std::string const& current_room, json_t* root_json)
{
    if(!add_pair_to_json_object(root_json, "game_name", game_name)) {
        game_error::fatal_error("Cannot create savefile game_name string");
        json_decref(root_json);
        return false;
    }

    if(!add_pair_to_json_object(root_json, "current_room", current_room)) {
        game_error::fatal_error("Cannot create savefile game_name string");
        return false;
    }

    if(!set_game_vars_json_array(player.gvars, root_json)) {
        return false;
    }

    if(!set_items_json_array(player.inv, root_json)) {
        return false;
    }

    return true;
}

static bool write_save_to_file(std::string const& local_data_path,
        const json_t* json_root)
{
    const std::string file_path = local_data_path + "save.json";

    if(json_dump_file(json_root, file_path.c_str(), JSON_INDENT(4)) != 0) {
        game_error::fatal_error("Cannot write savefile JSON to file");
        return false;
    } else {
        return true;
    }
}

bool savefile::save(Player const& player, std::string const& current_room,
        std::string const& local_data_path)
{
    json_t* root_json;

    fileio::create_directories(local_data_path);

    root_json = json_object();
    if(!root_json) {
        game_error::fatal_error("Cannot create savefile JSON root object");
        return false;
    }

    if(!set_json_structure(player, current_room, root_json)) {
        return false;
    } else if(!write_save_to_file(local_data_path, root_json)) {
        json_decref(root_json);
        return false;
    }

    json_decref(root_json);

    return true;
}

static void check_int32_overflow(json_int_t* var)
{
    if(*var > INT32_MAX) {
        *var = INT32_MAX;
    }
}

static bool parse_json_inventory_items(savefile::load_data& savefile_data,
        json_t const* root_json)
{
    json_t* items_array;
    json_t* item;
    size_t i;

    items_array = json_object_get(root_json, "items");
    json_array_foreach(items_array, i, item) {
        json_error_t error;
        json_int_t number = 0;
        inventory::gitem new_gitem;
        const char* id;

        if(json_unpack_ex(item, &error, 0, "{ s:s, s:i }", "id", &id, "number",
                    &number) != 0) {
            game_error::fatal_error("Cannot get savefile JSON inventory item: "
                    + std::string(error.text) + " (line "
                    + std::to_string(error.line) + ")");
            return false;
        } else {
            check_int32_overflow(&number);

            new_gitem.name = id;
            new_gitem.val = number;

            savefile_data.gitems.push_back(std::move(new_gitem));
        }
    }

    return true;
}

static bool parse_json_game_vars(savefile::load_data& savefile_data,
        json_t const* root_json)
{
    json_t* gvars_array;
    json_t* gvar;
    size_t i;

    gvars_array = json_object_get(root_json, "game_vars");
    json_array_foreach(gvars_array, i, gvar) {
        json_error_t error;
        json_int_t value = 0;
        const char* id;

        if(json_unpack_ex(gvar, &error, 0, "{ s:s, s:i }", "id", &id, "value",
                    &value) != 0) {
            game_error::fatal_error("Cannot get savefile JSON game var: "
                    + std::string(error.text) + " (line "
                    + std::to_string(error.line) + ")");
        } else {
            check_int32_overflow(&value);
            gvars::set_var(savefile_data.gvars, std::move(id), value);
        }
    }

    return true;
}

static bool parse_json_structure(savefile::load_data& savefile_data,
        json_t const* root_json)
{
    std::string game_name_str;
    json_t* game_name_json;
    json_t* current_room_json;

    game_name_json = json_object_get(root_json, "game_name");
    if(!game_name_json || game_name_json->type != JSON_STRING) {
        savefile_data.error = savefile::loading_error::BAD_FORMAT;
        return false;
    }
    game_name_str = json_string_value(game_name_json);
    if(game_name_str != game_name) {
        savefile_data.error = savefile::loading_error::BAD_GAME_NAME;
        return false;
    }

    current_room_json = json_object_get(root_json, "current_room");
    if(!current_room_json || current_room_json->type != JSON_STRING) {
        savefile_data.error = savefile::loading_error::BAD_FORMAT;
        return false;
    }
    savefile_data.current_room = json_string_value(current_room_json);

    return parse_json_inventory_items(savefile_data, root_json)
        && parse_json_game_vars(savefile_data, root_json);
}

bool savefile::load(savefile::load_data& savefile_data,
        std::string const& local_data_path)
{
    const std::string file_path = local_data_path + "save.json";
    json_t* root_json;
    json_error_t error;

    savefile_data.error = loading_error::NOERROR;

    root_json = json_load_file(file_path.c_str(), JSON_REJECT_DUPLICATES,
            &error);
    if(!root_json) {
        game_error::fatal_error("Cannot parse savefile json file: "
                + std::string(error.text) + " (line "
                + std::to_string(error.line) + ")");
        return false;
    } else if(!parse_json_structure(savefile_data, root_json)) {
        json_decref(root_json);
        return false;
    }

    json_decref(root_json);

    return true;
}
