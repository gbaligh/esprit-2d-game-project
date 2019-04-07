
CFLAGS:= -std=gnu99 -I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -g -O2 -L/usr/lib/x86_64-linux-gnu
LDFLAGS:= -lSDL  -lSDL_gfx -lSDL_mixer -lSDL_ttf -lSDL_image -lm

all:
	gcc -o game main.c col.c hero.c $(CFLAGS) $(LDFLAGS)
