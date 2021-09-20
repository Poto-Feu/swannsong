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

#include <cstdio>
#include <filesystem>

#include "files_path.hpp"
#include "os_module.hpp"

using namespace os_module;

static std::string init_data_path(bool is_local)
{
#ifdef IS_PKG
    return "/usr/share/swannsong_adventure/";
#else
    if(current_os == os_type::WINDOWSNT || is_local) {
        return "data/";
    } else return "/usr/local/share/swannsong_adventure/";
#endif
}

static void print_wrong_os_error()
{
    puts("Please use a POSIX or a WINDOWS NT environnement to run this "
            "program");
}

static std::string init_local_data_path()
{
    std::string local_data_path;

    if(current_os == os_type::UNIXLIKE) {
        local_data_path = std::getenv("HOME");
        local_data_path += "/.local";
        files_path::create_directory(local_data_path);
        local_data_path += "/share";
        files_path::create_directory(local_data_path);
        local_data_path += "/swannsong_adventure/";
        files_path::create_directory(local_data_path);
    } else if(current_os == os_type::WINDOWSNT) {
        local_data_path = std::getenv("LOCALAPPDATA");

        local_data_path += "/swannsong_adventure/";
        files_path::create_directory(local_data_path);
    } else {
        print_wrong_os_error();
        std::exit(1);
    }
    return local_data_path;
}

static std::string init_local_conf_path()
{
    std::string local_conf_path;

    if(current_os == os_type::UNIXLIKE) {
        local_conf_path = std::getenv("HOME");
        local_conf_path += "/.config";
        files_path::create_directory(local_conf_path);
        local_conf_path += "/swannsong_adventure/";
        files_path::create_directory(local_conf_path);
    } else if(current_os == os_type::WINDOWSNT) {
        local_conf_path = std::getenv("LOCALAPPDATA");

        local_conf_path += "/swannsong_adventure/";
        files_path::create_directory(local_conf_path);
        local_conf_path += "/config/";
        files_path::create_directory(local_conf_path);
    } else {
        print_wrong_os_error();
        std::exit(1);
    }
    return local_conf_path;
}

bool files_path::create_directory(std::string const& p_path)
{
    if(!std::filesystem::exists(p_path)) {
        std::filesystem::create_directory(p_path);
        return true;
    } else return false;
}

/* Return a struct containing the system game data path and the user-specific
 * game data path */
files_path::paths_struct files_path::getpaths(bool is_local)
{
    paths_struct rtrn_struct;

    rtrn_struct.data_path = init_data_path(is_local);
    rtrn_struct.local_data_path = init_local_data_path();
    rtrn_struct.local_conf_path = init_local_conf_path();
    return rtrn_struct;
}

std::string files_path::get_local_data_path()
{
    return init_local_data_path();
}
