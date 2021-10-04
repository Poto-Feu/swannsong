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

#include <cstring>
#include <unordered_map>

extern "C" {
#include "jansson.h"
}

#include "cutsecenes.hpp"
#include "game_error.hpp"

struct cutscenes::csdata {
    std::unordered_map<std::string, Cutscene> cs_map;
};

static bool parse_cutscene_field_json(CutsceneField& cutscene_field,
        json_t* cutscene_field_json)
{
    const char* type;
    const char* value;
    json_error_t error;

    if(json_unpack_ex(cutscene_field_json, &error, 0, "{s:s s:s}", "type",
                &type, "value", &value) != 0) {
        game_error::fatal_error("Cutscene field JSON error: " +
                std::string(error.text) + "(line " + std::to_string(error.line)
                + ")");
        return false;
    } else if(std::strcmp(type, "string") == 0) {
        cutscene_field.type = CutsceneFieldType::STRING;
        cutscene_field.content = value;
    } else if(std::strcmp(type, "blank") == 0) {
        cutscene_field.type = CutsceneFieldType::BLANK;
    } else {
        game_error::emit_warning("Unknown cutscene field JSON type (line "
                + std::to_string(error.line) + ")");
    }

    return true;
}

static bool parse_cutscene_screen_json(Cutscene& cutscene,
        json_t* cutscene_screen_json)
{
    json_t* cutscene_field_json;
    size_t i;

    json_array_foreach(cutscene_screen_json, i, cutscene_field_json) {
        CutsceneField cutscene_field;

        if(!parse_cutscene_field_json(cutscene_field, cutscene_field_json)) {
            return false;
        } else {
            cutscene.actions_vec.push_back(std::move(cutscene_field));
        }
    }

    return true;
}

static bool parse_individual_cutscene_json(
        cutscenes::csdata_ptr const& cs_data, std::string const& name,
        json_t* cutscene_json)
{
    json_t* cutscene_screen_json;
    Cutscene cutscene;
    bool is_new_screen;
    size_t i;

    if(json_typeof(cutscene_json) != JSON_ARRAY) {
        game_error::fatal_error("Individual cutscene JSON is not an array");
        return false;
    }

    is_new_screen = false;
    cutscene.actions_vec.reserve(json_array_size(cutscene_json));

    json_array_foreach(cutscene_json, i, cutscene_screen_json) {
        if(is_new_screen) {
            cutscene.actions_vec.push_back( { CutsceneFieldType::PAUSE, ""} );
            is_new_screen = false;
        }

        if(!parse_cutscene_screen_json(cutscene, cutscene_screen_json)) {
            return false;
        } else {
            is_new_screen = true;
        }
    }

    cs_data->cs_map[name] = std::move(cutscene);

    return true;
}

static bool parse_cs_json(cutscenes::csdata_ptr const& cs_data, json_t* root)
{
    json_t* child_json;
    const char* key;

    if(json_typeof(root) != JSON_OBJECT) {
        game_error::fatal_error("root cutscenes JSON is not an object");
        return false;
    }

    json_object_foreach(root, key, child_json) {
        if(!parse_individual_cutscene_json(cs_data, key, child_json)) {
            return false;
        }
    }
    return true;
}

static bool load_cs_json(std::string const& game_data_path,
        cutscenes::csdata_ptr const& cs_data)
{
    const std::string file_path = game_data_path + "cutscenes.json";
    json_t* root_json;
    json_error_t error;
    std::string file_content;

    root_json = json_load_file(file_path.c_str(), JSON_REJECT_DUPLICATES,
            &error);
    if(!root_json) {
        game_error::fatal_error("Cannot load cutscenes JSON: "
                + std::string(error.text) + " (line "
                + std::to_string(error.line) + ")");
        return false;
    }

    if(!parse_cs_json(cs_data, root_json)) {
        json_decref(root_json);
        return false;
    } else {
        json_decref(root_json);
        return true;
    }
}

cutscenes::csdata_ptr cutscenes::init(std::string const& game_data_path)
{
    csdata_ptr cs_data(new cutscenes::csdata);

    if(!load_cs_json(game_data_path, cs_data)) {
        return nullptr;
    } else {
        return cs_data;
    }
}

Cutscene const* cutscenes::get(csdata_ptr const& cs_data,
        std::string const& id)
{
    auto const it = cs_data->cs_map.find(id);

    if(it == cs_data->cs_map.cend()) {
        return nullptr;
    } else {
        return &it->second;
    }
}
