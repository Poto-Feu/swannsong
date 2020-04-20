cmake_minimum_required(VERSION 3.0)

project(SwannSong)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(default_build_type "Release")

include_directories(src)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -pedantic)
else()
    message(WARNING "warnings flags will only be set in Clang or GCC")
endif()

add_executable(swannsong src/main.c
    src/room/room.c
    src/room/find.c
    src/room/room_io.cpp
    src/fileio/fileio.c
    src/fileio/gameconf.c
    src/interpreter/parser.c
    src/interpreter/token.c
    src/vars/pvars.c
    src/vars/intvar.c
    src/vars/gvars.c
    src/init.c
    src/perror.c
    src/pstrings.cpp
    src/stringsm.c
    src/exitgame.c
    src/inventory.c)

# Copy necessary text files
add_custom_command(TARGET swannsong POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/gameconf.txt
        ${CMAKE_CURRENT_BINARY_DIR}/gameconf.txt)
add_custom_command(TARGET swannsong POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/txt/
        ${CMAKE_CURRENT_BINARY_DIR}/txt/)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

option(TESTS "Enable unit testing linking" OFF)
if(TESTS)
    include_directories(tests)
    add_compile_definitions(UNIT_TESTING)
    target_sources(swannsong PRIVATE src/tests/tests.c)
endif()
