# SwannSong Adventure
A text-adventure game with some humor. Please note that this game is in a pre-alpha stage and is far from being finished.

**WARNING:** this game contains infrequent strong language.

## Building the game

### Building prerequisites

#### Unix-like (including GNU/Linux)
* GCC/Clang
* ncurses
* CMake

#### Windows
* MinGW
* CMake

Please note that while this program may be compiled with another toolchain, we cannot guarantee that it will work as intended.

### Compiling

#### Unix-like
```bash
mkdir build && cd build
cmake .. 
make
```

#### Windows
TO-DO


## Roadmap
* Refactor some parts of the source code
* Add essential features to the rooms file interpreter such as conditions
* Add room choice prompt
* Add a cutscene system
* Develop a storyline


## Licensing
**SwannSong Adventure** is licensed under the GPL v3 License. Libraries licenses can be found in the *licenses* folder.

ncurses is licensed under the MIT X11 License.

The following only applies to the MingGW compiled version :

winpthreads is licensed under the Zope Public License v2.1. PDCurses was developed by Mark Hessling and William McBrine and is in the public domain.
