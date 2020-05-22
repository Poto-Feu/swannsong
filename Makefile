CC = gcc
CXX = g++

LIBS = -lncurses
INC = -Isrc

WARN_FLAGS = -Wall -Wextra -pedantic
COMMON_FLAGS = -O2 $(WARN_FLAGS) $(INC)
CXXFLAGS = -std=gnu++14 $(COMMON_FLAGS)
CFLAGS = -std=gnu99

LDFLAGS = $(LIBS)

CCSRC = $(wildcard src/*.c)
CXXSRC = $(wildcard src/*.cpp) \
	$(wildcard src/fileio/*.cpp) \
	$(wildcard src/room/*.cpp) \
	$(wildcard src/room/interpreter/*.cpp) \
	$(wildcard src/tests/*.cpp) \
	$(wildcard src/vars/*.cpp)

OBJ = $(CCSRC:.c=.o) $(CXXSRC:.cpp=.o)

swannsongadv: $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf *.o && rm -f swannsongadv
	rm -rf src/*.o
	rm -rf src/fileio/*.o
	rm -rf src/room/*.o
	rm -rf src/room/interpreter/*.o
	rm -rf src/tests/*.o
	rm -rf src/vars/*.o
