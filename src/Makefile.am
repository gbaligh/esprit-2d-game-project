AM_CFLAGS      = -D_XOPEN_SOURCE=600 -D_BSD_SOURCE -D_GNU_SOURCE -D_REENTRANT
AM_LDFLAGS     =

sbin_PROGRAMS  = game
game_SOURCES  = main.c menu.c debug.c window.c
game_CFLAGS   = -W -Wall -Wextra -std=gnu99 $(SDL_CFLAGS) 
game_LDADD    = $(SDL_LIBS) 
