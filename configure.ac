AC_INIT([swannsongadv], 0.2)
AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_17(ext)

AC_DEFUN([SWSA_MSG_ERROR],
	[AC_MSG_ERROR(["$1" library is needed to build this program])]
)

AC_SEARCH_LIBS([json_object_set_new], [jansson], [],
	[SWSA_MSG_ERROR([jansson])]
)
AC_SEARCH_LIBS([noecho], [ncursesw], [],
	[SWSA_MSG_ERROR([ncursesw])]
)

AC_DEFINE(GAME_VERSION, PACKAGE_VERSION)

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
