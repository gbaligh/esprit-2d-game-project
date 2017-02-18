#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "debug.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "SDL game";

struct button_s {
  const char *name;
  SDL_Surface *imageSurface;
  SDL_Surface *textSurface;
};

int main(int argc, char *argv[])
{
  int gameRunning = 1;
  SDL_Surface *screen;
  SDL_Event event;
  const SDL_VideoInfo* screenInfo = NULL;
  int screenWidth = WINDOW_WIDTH;
  int screenHeight = WINDOW_HEIGHT;

  /* Init SDL */
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    log_error("Could not initialize SDL");
    return 128;
  }

  if (TTF_Init() != 0) {
    log_error("Error initializing TTF: %s", TTF_GetError());
    SDL_Quit();
    return 128;
  }

  SDL_WM_SetCaption(WINDOW_TITLE, NULL);
  //SDL_WM_SetIcon(SDL_LoadBMP(icon),NULL);

  screenInfo = SDL_GetVideoInfo();
  screenWidth = screenInfo->current_w;
  screenHeight = screenInfo->current_h;
  
  screen = SDL_SetVideoMode( 0 /*screenWidth*/, 0 /*screenHeight*/, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  if (screen == NULL) {
    log_error("Error creating window");
    TTF_Quit();
    SDL_Quit();
    return 128;
  }

  while (gameRunning) {
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        gameRunning = 0;
      }
    }

    menu_init(screen);

    SDL_Flip(screen);
  }

  TTF_Quit();
  SDL_Quit();
  return 0;
}

// vim: ts=2:sw=2:expandtab
