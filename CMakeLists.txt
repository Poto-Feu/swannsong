cmake_minimum_required(VERSION 3.0)

project(SwannSong)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -pedantic)
else()
    message(WARNING "compiler and linker flags are only set if Clang or GCC is 
    used")
endif()

add_executable(swannsongadv
    src/main.cpp
    src/room/interpreter/parser.cpp
    src/room/interpreter/token.cpp
    src/room/Choice.cpp
    src/room/room.cpp
    src/room/RoomClass.cpp
    src/room/RoomManager.cpp
    src/room/find.cpp
    src/room/room_io.cpp
    src/fileio/fileio.cpp
    src/fileio/gameconf.cpp
    src/vars/pvars.cpp
    src/vars/intvar.cpp
    src/vars/gvars.cpp
    src/tests/tests.cpp
    src/cutscenes.cpp
    src/init.cpp
    src/perror.c
    src/pstrings.cpp
    src/pcurses.cpp
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

# Copy necessary text files
add_custom_command(TARGET swannsongadv PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/gameconf.txt
        ${CMAKE_CURRENT_BINARY_DIR}/gameconf.txt)
add_custom_command(TARGET swannsongadv PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/txt/
        ${CMAKE_CURRENT_BINARY_DIR}/txt/)
add_custom_command(TARGET swannsongadv PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/licenses/
        ${CMAKE_CURRENT_BINARY_DIR}/licenses/)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()
