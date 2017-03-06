#pragma once
#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <stdbool.h>
#include "SDL.h"

bool init_window(const char* title, int width, int height, bool fullscreen);

void dispose_window(void);

SDL_Surface* get_screen(void);

void clear_screen(int r, int g, int b, int a);

void flip_screen(void);
#endif
// vim: ts=2:sw=2:expandtab
