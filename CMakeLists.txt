cmake_minimum_required(VERSION 3.10)

# set the project name
project(SwannSong VERSION 0.1)

# add the executable
add_executable(swannsong src/main.c
    src/room/room.c
    src/room/find.c
    src/fileio/fileio.c
    src/fileio/gameconf.c
    src/fileio/parser.c
    src/vars/pvars.c
    src/vars/pconst.c
    src/init.c
    src/perror.c
    src/pstrings.c
    src/stringsm.c
    src/exitgame.c)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_FLAGS "-pedantic -Wall -Wextra")
