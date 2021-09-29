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

#include <fstream>

extern "C" {
#include <jansson.h>
}

#include "savefile.hpp"
#include "fileio/fileio.hpp"
#include "game_error.hpp"
#include "player/Player.hpp"

static const std::string const_game_name = "SwannSong Adventure";

static bool open_save_file_write(std::ofstream& file_stream,
        std::string const& local_data_path)
{
    file_stream.open(local_data_path + "save.json");

    if(!file_stream.is_open()) {
        game_error::fatal_error("Cannot open save file");
        return false;
    }

    return true;
}

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
        std::string const& key, int value)
{
    json_t* child = json_integer(value);

    if(!child) {
        game_error::fatal_error("Cannot create JSON int");
        json_decref(child);

        return false;
    }

    return add_to_json_object(object_json, key, child);
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

        gvar_json = json_object();
        if(!gvar_json) {
            game_error::fatal_error("Cannot create savefile item object");
            json_decref(gvars_array_json);
            return false;
        }

        if(!add_pair_to_json_object(gvar_json, "id", it.name)) {
            json_decref(gvar_json);
            json_decref(gvars_array_json);
            return false;
        }

        if(!add_pair_to_json_object(gvar_json, "value", it.val)) {
            game_error::fatal_error("Cannot create jannson int");
            json_decref(gvar_json);
            json_decref(gvars_array_json);
            return false;
        }

        if(json_array_append_new(gvars_array_json, gvar_json) == -1) {
            game_error::fatal_error("Cannot add game_var to savefile array");
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

        item_json = json_object();
        if(!item_json) {
            game_error::fatal_error("Cannot create savefile item object");
            json_decref(items_array_json);
            return false;
        }

        if(!add_pair_to_json_object(item_json, "id", it.name)) {
            json_decref(item_json);
            json_decref(items_array_json);
            return false;
        }

        if(!add_pair_to_json_object(item_json, "number", it.val)) {
            json_decref(item_json);
            json_decref(items_array_json);
            return false;
        }

        if(json_array_append_new(items_array_json, item_json) == -1) {
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
    if(!add_pair_to_json_object(root_json, "game_name", const_game_name)) {
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

static bool write_save_to_file(std::ofstream& file_stream,
        const json_t* json_root)
{
    char *buf_c;
    std::string buf;

    buf_c = json_dumps(json_root, JSON_INDENT(4));
    if(!buf_c) {
        game_error::fatal_error("Cannot dump savefile JSON structure");
        return false;
    }

    buf = buf_c;
    buf += '\n';
    free(buf_c);

    file_stream << buf;

    return true;
}

bool savefile::save(Player const& player, std::string const& current_room,
        std::string const& local_data_path)
{
    std::ofstream file_stream;
    json_t* root_json;

    fileio::create_directories(local_data_path);

    if(!open_save_file_write(file_stream, local_data_path)) {
        return false;
    }

    root_json = json_object();
    if(!root_json) {
        game_error::fatal_error("Cannot create savefile root object");
        return false;
    }

    if(!set_json_structure(player, current_room, root_json)) {
        return false;
    }

    if(!write_save_to_file(file_stream, root_json)) {
        json_decref(root_json);
        return false;
    }

    json_decref(root_json);

    return true;
}

static bool open_save_file_load(savefile::load_data& savefile_data,
        std::string const& local_data_path, std::string& content)
{
    if(!fileio::copy_to_string(local_data_path + "save.json", content)) {
        savefile_data.error = savefile::loading_error::NO_FILE;
        return false;
    }

    return true;
}

static bool parse_json_inventory_items(savefile::load_data& savefile_data,
        json_t const* root_json)
{
    using namespace savefile;

    json_t* items_array;
    json_t* item;
    size_t i;

    items_array = json_object_get(root_json, "items");
    json_array_foreach(items_array, i, item) {
        inventory::gitem new_gitem;
        std::string id_str;
        uint32_t number_int;
        json_t* id_json;
        json_t* number_json;

        id_json = json_object_get(item, "id");
        if(!id_json || id_json->type != JSON_STRING) {
            savefile_data.error = loading_error::BAD_FORMAT;
            return false;
        }
        id_str = json_string_value(id_json);

        number_json = json_object_get(item, "number");
        if(!number_json || number_json->type != JSON_INTEGER) {
            savefile_data.error = loading_error::BAD_FORMAT;
            return false;
        }
        number_int = json_integer_value(number_json);

        new_gitem.name = std::move(id_str);
        new_gitem.val = number_int;

        savefile_data.gitems.push_back(std::move(new_gitem));
    }

    return true;
}

static bool parse_json_game_vars(savefile::load_data& savefile_data,
        json_t const* root_json)
{
    using namespace savefile;

    json_t* gvars_array;
    json_t* gvar;
    size_t i;

    gvars_array = json_object_get(root_json, "game_vars");
    json_array_foreach(gvars_array, i, gvar) {
        std::string id_str;
        uint32_t value_int;
        json_t* id_json;
        json_t* value_json;

        id_json = json_object_get(gvar, "id");
        if(!id_json || id_json->type != JSON_STRING) {
            savefile_data.error = loading_error::BAD_FORMAT;
            return false;
        }
        id_str = json_string_value(id_json);

        value_json = json_object_get(gvar, "value");
        if(!value_json|| value_json->type != JSON_INTEGER) {
            savefile_data.error = loading_error::BAD_FORMAT;
            return false;
        }
        value_int = json_integer_value(value_json);

        gvars::set_var(savefile_data.gvars, std::move(id_str), value_int);
    }

    return true;
}

static bool parse_json_structure(savefile::load_data& savefile_data,
        json_t const* root_json)
{
    using namespace savefile;

    std::string game_name_str;
    json_t* game_name_json;
    json_t* current_room_json;

    game_name_json = json_object_get(root_json, "game_name");
    if(!game_name_json || game_name_json->type != JSON_STRING) {
        savefile_data.error = loading_error::BAD_FORMAT;
        return false;
    }
    game_name_str = json_string_value(game_name_json);
    if(game_name_str != const_game_name) {
        savefile_data.error = loading_error::BAD_GAME_NAME;
        return false;
    }

    current_room_json = json_object_get(root_json, "current_room");
    if(!current_room_json || current_room_json->type != JSON_STRING) {
        savefile_data.error = loading_error::BAD_FORMAT;
        return false;
    }
    savefile_data.current_room = json_string_value(current_room_json);

    return parse_json_inventory_items(savefile_data, root_json)
        && parse_json_game_vars(savefile_data, root_json);
}

bool savefile::load(savefile::load_data& savefile_data,
        std::string const& local_data_path)
{
    json_t* root_json;
    std::string content;

    savefile_data.error = loading_error::NOERROR;

    if(!open_save_file_load(savefile_data, local_data_path, content)) {
        return false;
    }

    root_json = json_loads(content.c_str(), JSON_REJECT_DUPLICATES, NULL);
    if(!root_json) {
        return false;
    }

    if(!parse_json_structure(savefile_data, root_json)) {
        json_decref(root_json);
        return false;
    }

    json_decref(root_json);

    return true;
}
