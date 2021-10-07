AM_CFLAGS = -I$(top_srcdir)/src/
AM_CXXFLAGS = -I$(top_srcdir)/src/

EXTRA_DIST = README.md

bin_PROGRAMS = swannsongadv

swannsongadv_SOURCES = \
	src/CutsceneClass.hpp \
	src/Game.cpp \
	src/Game.hpp \
	src/LocalConfVars.cpp \
	src/LocalConfVars.hpp \
	src/cutsecenes.cpp \
	src/cutsecenes.hpp \
	src/dialogbox.cpp \
	src/dialogbox.hpp \
	src/display_server.cpp \
	src/display_server.hpp \
	src/fileio.cpp \
	src/fileio.hpp \
	src/files_path.cpp \
	src/files_path.hpp \
	src/game_data.cpp \
	src/game_error.cpp \
	src/game_error.hpp \
	src/game_lang.cpp \
	src/game_lang.hpp \
	src/game_menu.cpp \
	src/game_menu.hpp \
	src/game_state.hpp \
	src/gameconf.cpp \
	src/gameconf.hpp \
	src/main.cpp \
	src/os_module.hpp \
	src/pargs.cpp \
	src/pargs.hpp \
	src/pcurses.cpp \
	src/pcurses.hpp \
	src/player/inventory.cpp \
	src/player/inventory.hpp \
	src/player/Player.hpp \
	src/pstrings.cpp \
	src/pstrings.hpp \
	src/rendering.cpp \
	src/rendering.hpp \
	src/room/Choice.cpp \
	src/room/Choice.hpp \
	src/room/RoomLoopState.cpp \
	src/room/RoomLoopState.hpp \
	src/room/RoomClass.cpp \
	src/room/RoomClass.hpp \
	src/room/RoomDisplay.hpp \
	src/room/interpreter/parser.cpp \
	src/room/interpreter/parser.hpp \
	src/room/interpreter/token.cpp \
	src/room/interpreter/token.hpp \
	src/room/room_parsing.cpp \
	src/room/room_parsing.hpp \
	src/room/rooms.cpp \
	src/room/rooms.hpp \
	src/savefile.cpp \
	src/savefile.hpp \
	src/stringsm.cpp \
	src/stringsm.hpp \
	src/terminal.cpp \
	src/terminal.hpp \
	src/userio.cpp \
	src/userio.hpp \
	src/vars/gvars.cpp \
	src/vars/gvars.hpp \
	src/vars/intvar.cpp \
	src/vars/intvar.hpp

dist_man_MANS = swannsongadv.6

gamedir = $(datadir)/swannsong_adventure

dist_game_DATA = \
	data/cutscenes.json \
	data/gameconf.json \
	data/rooms.txt

langdir = $(gamedir)/lang
dist_lang_DATA = \
	data/lang/eng.txt \
	data/lang/fra.txt
