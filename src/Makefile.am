AM_CPPFLAGS    = -D_XOPEN_SOURCE=600 -D_BSD_SOURCE -D_GNU_SOURCE
AM_LDFLAGS     =

sbin_PROGRAMS  = game
game_SOURCES  = menu.c
game_CFLAGS   = -W -Wall -Wextra -std=gnu99 $(sdl_CFLAGS) $(sdlmain_CFLAGS) $(sdl_ttf_CFLAGS) $(sdl_mixer_CFLAGS) $(sdl_image_CFLAGS)
game_LDADD    = $(sdl_LIBS) $(sdlmain_LIBS) $(sdl_ttf_LIBS) $(sdl_mixer_LIBS) $(sdl_image_LIBS)
