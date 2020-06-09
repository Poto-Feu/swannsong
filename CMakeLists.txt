cmake_minimum_required(VERSION 3.12)

project(SwannSong)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)

set(src_data_folder ${CMAKE_CURRENT_SOURCE_DIR}"/game_data")
set(build_data_folder ${CMAKE_CURRENT_BINARY_DIR}"/data")

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
    src/room/interpreter/parser.cpp
    src/room/interpreter/token.cpp
    src/room/Choice.cpp
    src/room/room.cpp
    src/room/RoomClass.cpp
    src/room/RoomManager.cpp
    src/room/RoomState.cpp
    src/room/find.cpp
    src/room/room_io.cpp
    src/fileio/fileio.cpp
    src/fileio/gameconf.cpp
    src/vars/intvar.cpp
    src/vars/gvars.cpp
    src/tests/tests.cpp
    src/cutscenes.cpp
    src/files_path.cpp
    src/display_server.cpp
    src/init.cpp
    src/lang.cpp
    src/pargs.cpp
    src/perror.c
    src/pstrings.cpp
    src/pcurses.cpp
    src/os_module.cpp
    src/stringsm.cpp
    src/exitgame.cpp
    src/inventory.cpp
    src/userio.cpp)

# Compile pdcurses if the user is on windows
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIRS})
    target_link_libraries(swannsongadv ${CURSES_LIBRARIES})
else()
    add_subdirectory(lib/pdcurses)
    include_directories(lib/pdcurses)
    target_link_libraries(swannsongadv pdcurses)
endif()

# Statically link libraries if compiled on MinGW (GCC C++ libs are rarely
# installed on Windows)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
        AND CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(swannsongadv
        -static)

    add_custom_command(TARGET swannsongadv POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/lib/pdcurses/libpdcurses.dll
            ${CMAKE_CURRENT_BINARY_DIR}/libpdcurses.dll)
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
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/data/
        DESTINATION share/swannsong_adventure/
        PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
endif()

add_custom_command(TARGET swannsongadv POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${src_data_folder}
        ${build_data_folder})

add_custom_command(TARGET swannsongadv POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/licenses/
        ${CMAKE_CURRENT_BINARY_DIR}/licenses/)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()
