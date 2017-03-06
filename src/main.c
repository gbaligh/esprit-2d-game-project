#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "debug.h"
#include "window.h"
#include "menu.h"
#include "main.h"

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "SDLv1.2 game for education";
int gameRunning = 1;

static SDL_Event event;

static struct main_obj_s *objs[10];

static void main_process_event(void)
{
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT:
        gameRunning = 0;
        break;

      default:
        break;
    }
  }
}

static void main_loop(void)
{
  unsigned int _i = 0;

  log_info("Starting loop ...");
  
  while (gameRunning) {
    main_process_event();
    for (_i = 0; _i < ARRAY_SIZE(objs); ++_i) {
      if (objs[_i]) {
        if (objs[_i]->obj_event_handler) {
          objs[_i]->obj_event_handler(&event);
        }
        if (objs[_i]->obj_blit) {
          objs[_i]->obj_blit(get_screen());
        }
      }
    }
    flip_screen();
  }
}

static int main_init(void)
{
  SDL_version compiled;

  /* Init SDL */
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    log_error("Could not initialize SDL");
    return 1;
  }
  
  SDL_VERSION(&compiled);
  log_info("Compiled against SDL %d.%d.%d", compiled.major, compiled.minor, compiled.patch);
  
  srand(SDL_GetTicks());

  /* Init IMG for PNG loading */
  if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
    log_error("Failed to init SDL_image - PNG", IMG_GetError());
    goto ERROR_L1;
  }

  /* Init TTF */
  if (TTF_Init() < 0) {
    log_error("Error initializing TTF: %s", TTF_GetError());
    goto ERROR_L2;
  }

  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) {
    log_error("Error OpenAudio: %s", Mix_GetError());
  }
  
  if (menu_init() != 0) {
    log_error("Menu initialization");
    goto ERROR_L3;
  }

  return 0;

ERROR_L3:
  TTF_Quit();
ERROR_L2:
  IMG_Quit();
ERROR_L1:
  SDL_Quit();
  return 1;
}

static void main_deinit()
{
  log_info("Destroy all reserved memory");
  menu_deinit();
  Mix_CloseAudio();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

int main_register_obj(struct main_obj_s *pObj)
{
  unsigned int _i = 0;
  for (_i = 0; _i < ARRAY_SIZE(objs); ++_i) {
    if (objs[_i] == NULL) {
      objs[_i] = malloc(sizeof(struct main_obj_s));
      memcpy(objs[_i], pObj, sizeof(struct main_obj_s));
      return 0;
    }
  } 
  return 1;
}

int main_unregister_obj(struct main_obj_s *pObj)
{
  unsigned int _i = 0;
  for (_i = 0; _i < ARRAY_SIZE(objs); ++_i) {
    if (objs[_i] == pObj) {
      free(objs[_i]);
      objs[_i] = NULL;
      return 0;
    }
  } 
  return 1;
}

int main_isRunning(void)
{
  return gameRunning;
}

void main_loop_stop(void)
{
  gameRunning = 0;
}

int main(int argc, char *argv[])
{
  if (main_init() != 0) {
    log_error("Initialisation failed");
    return 128;
  }

  init_window(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT, true);

  main_loop();
  
  dispose_window();
  main_deinit();
  return 0;
}

// vim: ts=2:sw=2:expandtab
