AC_PREREQ([2.69])
AC_INIT([ESPRIT-2D-GAME], [1.0], [https://github.com/gbaligh/esprit-2d-game-project/issues])

AM_INIT_AUTOMAKE([1.11 foreign no-dist-gzip dist-xz subdir-objects])
AM_SILENT_RULES([yes])

AC_CONFIG_SRCDIR([src/menu.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.

# Check for required libraries (-lSDLmain -lSDL -lSDL_ttf -lSDL_mixer -lSDL_image)
PKG_CHECK_MODULES([sdl],  [libSDL])
PKG_CHECK_MODULES([sdlmain],  [libSDLmain])
PKG_CHECK_MODULES([sdl_ttf],  [libSDL_ttf])
PKG_CHECK_MODULES([sdl_mixer],  [libSDL_mixer])
PKG_CHECK_MODULES([sdl_image],  [libSDL_image])

AC_OUTPUT
