#include "debug.h"
#include "window.h"

static SDL_Surface *screen;

bool init_window(const char* title, int width, int height, bool fullscreen)
{
  const SDL_VideoInfo *_pScreenInfo = NULL;
  int screenWidth = width;
  int screenHeight = height;
  int flags = SDL_SWSURFACE|SDL_DOUBLEBUF;

  if (fullscreen)
    flags |= SDL_FULLSCREEN;

  _pScreenInfo = SDL_GetVideoInfo();
  screenWidth = _pScreenInfo->current_w;
  screenHeight = _pScreenInfo->current_h;

  screen = SDL_SetVideoMode(screenWidth, screenHeight, 32, flags);
  if (screen == NULL) {
    log_error("Error SetVideoMode for screen: %s", SDL_GetError());
    return false;
  }

  SDL_WM_SetCaption(title, NULL);

  log_info("FullScreen: Yes, Screen Width: %d, Screen Height: %d", screenWidth, screenHeight);

  return true;
}

void dispose_window(void)
{
  if (screen != NULL) {
    SDL_FreeSurface(screen);
  }
}

SDL_Surface *get_screen(void)
{
  return screen;
}

void clear_screen(int r, int g, int b, int a)
{
  Uint32 col = 0;
  
  if (screen == NULL)
    return;

  col = SDL_MapRGBA(screen->format, r, g, b, a);

  SDL_FillRect(screen, NULL, col);
}

void flip_screen(void)
{
  if (screen != NULL) {
    SDL_Flip(screen);
  }
}
// vim: ts=2:sw=2:expandtab
