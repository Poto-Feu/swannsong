# SwannSong
A text-adventure game with some humour. Please note that this game is in a pre-alpha stage and is far from being finished.

## Building the game

### Building prerequisites

#### Unix-like (including GNU/Linux)
* glibc
* gcc
* cmake

#### Windows
* mingw
* cmake


### Compiling

#### Unix-like
```bash
mkdir build && cd build
cmake .. 
cp -r ../txt ../gameconf.txt .
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


## License
**SwannSong** is licensed under the GPL v3 License.
