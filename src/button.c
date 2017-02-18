#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#define BUTTON_MAX_NAME_LEN 12

#define BUTTON_IMG_FILE_NAME "button"

struct button_s {
  const char *name;
  SDL_Surface *imageSurface;
  SDL_Surface *textSurface;
};

typedef struct button_s button_t;

button_t *button_create(const char *name, const char *file)
{
  struct button_s *_pButton = NULL;

  _pButton = (struct button_s *) malloc(sizeof(struct button_s));
  if (_pButton == NULL) {
    log_error("memory allocation");
    return NULL;
  }

  _pButton->name = strdup(name);
  if (_pButton->name == NULL) {
    log_error("memory allocation");
    return NULL;
  }

  _pButton->imageSurface = IMG_Load(BUTTON_IMG_FILE_NAME);
  if(_pButton->imageSurface == NULL) {
    log_error("Could not load IMG file(%s):[%s]", BUTTON_IMG_FILE_NAME, IMG_GetError());
    free(_pButton->name);
    free(_pButton);
    return NULL;
  }


  return _pButton;
}

// vim: ts=2:sw=2:expandtab
