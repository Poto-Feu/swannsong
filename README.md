# SwannSong Adventure
A text-adventure game with some humor. Please note that this game is still in a pre-alpha stage.

This first started as an hobby project to improve my C++ skills but feel free to explore the source.

Note: do not try to load a save from a previous pre-alpha version (< 1.0) as it may cause problems. However, you can safely overwrite it.


## Building the game

### Building prerequisites

* a C++17-compatible version of GCC/MinGW
* CMake
* jansson
* ncursesw

Please note that while this program may be compiled with another toolchain, we cannot guarantee that it will work as intended.

### Compiling

#### Unix-like
```bash
mkdir build && cd build
cmake .. 
make
```

On Unix-like systems, you will need to use the `-local` arguments when launching the game from the build directory (or simply make the start.sh file executable with `chmod +x` and use it). A `make install` command is also provided. However, it is still in an experimental state.

#### Windows
TO-DO


## Usage
Note : On Windows, the program must be run from the executable directory. On Unix-like, if the program is not installed, it must be run in the build directory using the -local argument.

### Command-line arguments
* `-local`: use the game data from the current directory (this is mainly useful for running the program in its build directory without installing it)
* `-debug`: enable debug features such as warning logs (mainly for developers).
* `-reset`: remove the game user configuration as if it was a fresh install.


## Roadmap
* Refactor some parts of the source code
* Develop a storyline


## Licensing
**SwannSong Adventure** is licensed under the GNU General Public License Version 3. Third-party libraries licenses can be found in the *licenses* folder.
