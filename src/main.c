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
int gameRunning = 1;

struct button_s {
  const char *name;
  SDL_Surface *imageSurface;
  SDL_Surface *textSurface;
};

int main(int argc, char *argv[])
{
  SDL_Surface *_pScreen = NULL;
  SDL_Event event;
  const SDL_VideoInfo* _pScreenInfo = NULL;
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

  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) {
    fprintf(stderr, "Error OpenAudio: %s\n", Mix_GetError());
  }

  SDL_WM_SetCaption(WINDOW_TITLE, NULL);
  //SDL_WM_SetIcon(SDL_LoadBMP(icon),NULL);

  _pScreenInfo = SDL_GetVideoInfo();
  screenWidth = _pScreenInfo->current_w;
  screenHeight = _pScreenInfo->current_h;
  
  _pScreen = SDL_SetVideoMode(screenWidth, screenHeight, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
  if (_pScreen == NULL) {
    log_error("Error creating window");
    TTF_Quit();
    SDL_Quit();
    return 128;
  }

  log_info("FullScreen: Yes, Screen Width: %d, Screen Height: %d", screenWidth, screenHeight);

  if (Menu_Init() != 0) {
    log_error("Menu initialization");
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

    Menu_Action(&event);

    Menu_Blit(_pScreen);

    SDL_Flip(_pScreen);
  }

  Menu_Deinit();

  SDL_FreeSurface(_pScreen);

  TTF_Quit();
  SDL_Quit();
  return 0;
}

// vim: ts=2:sw=2:expandtab
