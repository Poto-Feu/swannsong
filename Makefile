CC = gcc
CXX = g++

LIBS = -lncurses -lcrypto
INC = -Isrc

WARN_FLAGS = -Wall -Wextra -pedantic
COMMON_FLAGS = -O2 $(WARN_FLAGS) $(INC)
CXXFLAGS = -std=gnu++17 $(COMMON_FLAGS)
CFLAGS = -std=gnu99

LDFLAGS = $(LIBS)

CCSRC = $(wildcard src/*.c)
CXXSRC = $(wildcard src/*.cpp) \
	$(wildcard src/fileio/*.cpp) \
	$(wildcard src/fileio/save/*.cpp) \
	$(wildcard src/room/*.cpp) \
	$(wildcard src/room/interpreter/*.cpp) \
	$(wildcard src/tests/*.cpp) \
	$(wildcard src/vars/*.cpp)

OBJ = $(CCSRC:.c=.o) $(CXXSRC:.cpp=.o)

swannsongadv: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(CFLAGS) $(LDFLAGS)
	mkdir build
	mkdir build/data
	mv swannsongadv build/swannsongadv
	cp -r game_data/. build/data/
	cp start.sh build/start.sh

.PHONY: clean distclean

clean:
	rm -rf *.o
	rm -rf src/*.o
	rm -rf src/fileio/*.o
	rm -rf src/fileio/save/*.o
	rm -rf src/room/*.o
	rm -rf src/room/interpreter/*.o
	rm -rf src/tests/*.o
	rm -rf src/vars/*.o

distclean: clean
	rm -rf build
