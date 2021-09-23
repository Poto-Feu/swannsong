cmake_minimum_required(VERSION 3.12)

project(SwannSong VERSION 0.3)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)

set(src_data_folder ${CMAKE_CURRENT_SOURCE_DIR}"/data")
set(build_data_folder ${CMAKE_CURRENT_BINARY_DIR}"/data")

add_compile_definitions(GAME_VERSION="${PROJECT_VERSION}")
add_compile_definitions(GAME_NAME="SwannSong Adventure")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(UNIX)
    if(IS_PKG)
        add_compile_definitions(IS_PKG)
    endif()

    set(executable_name "swannsongadv")
elseif(WIN32)
    set(executable_name "swannsongadv.exe")
else()
    message(FATAL_ERROR "Your OS is not supported by this CMake configuration. You must use a
    Unix-like or a Win32 environnement.")
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -pedantic)
else()
    message(WARNING "compiler and linker flags are only set if Clang or GCC is used")
endif()

add_executable(swannsongadv
    src/main.cpp
    src/fileio/conf_files.cpp
    src/fileio/fileio.cpp
    src/fileio/gameconf.cpp
    src/player/inventory.cpp
    src/room/interpreter/parser.cpp
    src/room/interpreter/token.cpp
    src/room/Choice.cpp
    src/room/RoomClass.cpp
    src/room/RoomLoopState.cpp
    src/room/RoomManager.cpp
    src/vars/intvar.cpp
    src/vars/gvars.cpp
    src/tests/tests.cpp
    src/CutscenesContainer.cpp
    src/LocalConfVars.cpp
    src/rendering.cpp
    src/dialogbox.cpp
    src/display_server.cpp
    src/files_path.cpp
    src/Game.cpp
    src/game_error.cpp
    src/game_menu.cpp
    src/pargs.cpp
    src/pcurses.cpp
    src/pstrings.cpp
    src/savefile.cpp
    src/terminal.cpp
    src/stringsm.cpp
    src/userio.cpp)

add_subdirectory(docs)

target_link_libraries(swannsongadv jansson ncursesw)

# Statically link libraries if compiled on MinGW (GCC C++ libs are rarely installed on Windows)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
        AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(swannsongadv -static-libgcc -static-libstdc++)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU"
        AND CMAKE_BUILD_TYPE STREQUAL "Release")
    target_link_libraries(swannsongadv
        -s)
endif()

if(UNIX)
    add_custom_command(TARGET swannsongadv POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/start.sh
        ${CMAKE_CURRENT_BINARY_DIR}/start.sh)

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${executable_name}
        DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/
        DESTINATION share/swannsong_adventure
        FILE_PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
endif()

add_custom_command(TARGET swannsongadv POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${src_data_folder}
        ${build_data_folder})

add_custom_command(TARGET swannsongadv POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/licenses/
        ${CMAKE_CURRENT_BINARY_DIR}/licenses/)
