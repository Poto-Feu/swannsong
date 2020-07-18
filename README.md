# SwannSong Adventure
A text-adventure game with some humor. Please note that this game is in a pre-alpha stage and is far from being finished.

I started this project in order to improve my skills in C++. Do not take source files from this program as an example of optimized and well-written code.

**WARNING:** this game contains infrequent strong language.

## Building the game

### Building prerequisites

#### Unix-like (including GNU/Linux)
* a C++17-compatible version of GCC or Clang
* ncurses
* OpenSSL libs
* CMake

#### Windows
* a C++17-compatible version of MinGW
* OpenSSL libs
* CMake

Please note that while this program may be compiled with another toolchain, we cannot guarantee that it will work as intended.

### Compiling

#### Unix-like
```bash
mkdir build && cd build
cmake .. 
make
```

On Unix-like systems, you will need to use the `-local` arguments when launching the game from the build directory (or simply make the start.sh file executable with `chmod +x` and use it). A `make install` command is also provided. However, it is still in an experimental state.

There is also a Makefile available for GNU Make (this is the default make on most GNU/Linux distributions - you may need to use gmake on some systems). Please note that it does not support the `make install` command yet.

#### Windows
TO-DO

Please note that the Windows version is tested with Wine.


## Usage

### Command-line arguments
* `-local`: fetch the game data from the current folder
* `-debug`: enable warning and error logs (mainly for developers)


## Roadmap
* Refactor some parts of the source code
* Develop a storyline


## Licensing
**SwannSong Adventure** is licensed under the GPL v3 License. Libraries licenses can be found in the *licenses* folder.

ncurses is licensed under the MIT X11 License. OpenSSL is licensed under the Apache License 2.0.

The following only applies to the MingGW compiled version :

winpthreads is licensed under the Zope Public License v2.1. PDCurses was developed by Mark Hessling and William McBrine and is in the public domain.
