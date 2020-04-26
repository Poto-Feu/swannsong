# SwannSong
A text-adventure game with some humour. Please note that this game is in a pre-alpha stage and is far from being finished.

## Building the game

### Building prerequisites

#### Unix-like (including GNU/Linux)
* Gcc/clang
* CMake

#### Windows
* Mingw
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
**SwannSong** is licensed under the GPL v3 License.

The MingGW compiled version of this program is statically linked with libstdc++ and libgcc. Both of these libraries are licensed under the GPL v3 License (with the GPL linking exception).
