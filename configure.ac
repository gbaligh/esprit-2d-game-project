AC_PREREQ([2.69])
AC_INIT([ESPRIT-2D-GAME], [1.0], [https://github.com/gbaligh/esprit-2d-game-project/issues])

AM_INIT_AUTOMAKE([1.11 foreign no-dist-gzip dist-xz subdir-objects])
AM_SILENT_RULES([yes])

AC_CONFIG_SRCDIR([src/menu.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.
AC_HEADER_STDBOOL

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC

# Check for SDL
SDL_VERSION=1.2.5
AM_PATH_SDL($SDL_VERSION,
	    :,
	    AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
	    )
AM_CFLAGS="$AM_CFLAGS $SDL_CFLAGS"
AM_LDFLAGS="$AM_LDFLAGS $SDL_LIBS"

# Check for SDL_image library
AC_CHECK_LIB(SDL_image, 
	     IMG_LoadPNG_RW, , 
	     AC_MSG_ERROR([
			*** Unable to find SDL_image libary with PNG support
			(http://www.libsdl.org/projects/SDL_image/)
			])
	     )

# Check for SDL_ttf library
AC_CHECK_LIB(SDL_ttf, 
	     TTF_Init, , 
	     AC_MSG_ERROR([
			*** Unable to find SDL_ttf libary
			(http://www.libsdl.org/projects/SDL_image/)
			])
	     )

# Check for SDL_mixer library
AC_CHECK_LIB(SDL_mixer, 
	     Mix_OpenAudio, , 
	     AC_MSG_ERROR([
			*** Unable to find SDL_mixer libary
			(http://www.libsdl.org/projects/SDL_image/)
			])
	     )

# Check for required libraries

AC_OUTPUT
