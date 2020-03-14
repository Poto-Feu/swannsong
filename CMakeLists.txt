cmake_minimum_required(VERSION 3.10)

# set the project name
project(SwannSong VERSION 0.1)

include_directories(src)

# add the executable
add_executable(swannsong src/main.c
    src/room/room.c
    src/room/find.c
    src/fileio/fileio.c
    src/fileio/gameconf.c
    src/fileio/parser.c
    src/vars/pvars.c
    src/init.c
    src/perror.c
    src/pstrings.c
    src/stringsm.c
    src/exitgame.c)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -Wall -Wextra -pedantic")
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
