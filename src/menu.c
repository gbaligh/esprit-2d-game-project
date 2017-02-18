#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include "debug.h"

#define SCREEN_WIDTH  357
#define SCREEN_HEIGHT 280
#define SCREEN_BPP 32

#define TTF_FONT_FILENAME "img/Roboto-Italic.ttf"
#define TTF_FONT_SIZE 64

#define IMG_BACKGROUND_FILENAME "img/background.png"
#define IMG_BUTTON_FILENAME "img/button.png"
#define IMG_BTN1_FILENAME "play.png"
#define IMG_BTN2_FILENAME "multiplayer.png"
#define IMG_BTN3_FILENAME "option.png"
#define IMG_BTN4_FILENAME "quit.png" 

#define MIX_EF_FILENAME "switch.wav"
#define MIX_BACK_FILENAME "back.mp3"

struct menu_s {
  SDL_Surface *menuSurface;
  TTF_Font *font;
};

SDL_Surface *menu_init(SDL_Surface *pScreen)
{
  SDL_Surface *_pOrigMenu = NULL;
  SDL_Surface *_pMenu = NULL;

  _pOrigMenu = IMG_Load(IMG_BACKGROUND_FILENAME);
  if (_pOrigMenu == NULL) {
    log_error();
    return NULL;
  }

  _pMenu = SDL_ConvertSurface(_pOrigMenu, pScreen->format, 0);
  if (_pMenu == NULL) {
    log_error();
    return NULL;
  } 

  SDL_FreeSurface(_pOrigMenu);

  SDL_BlitSurface(_pMenu, NULL, pScreen, NULL);

  menu_addButton(pScreen, "Play");

  //SDL_BlitSurface(_pMenu, NULL, pScreen, NULL);

  return _pMenu;
}

int menu_addButton(SDL_Surface *pMenu, const char *label)
{
  SDL_Surface *_pButton = NULL;
  TTF_Font *_pFont = NULL;
  SDL_Surface *_pText = NULL;
  SDL_Color blackColor = {0};
  SDL_Rect offset;

  _pButton = IMG_Load(IMG_BUTTON_FILENAME);
  if (_pButton == NULL) {
    log_error();
    return 1;
  }

  _pFont = TTF_OpenFont(TTF_FONT_FILENAME, TTF_FONT_SIZE);
  if (_pFont == NULL) {
    log_error("Loading font file(%s):[%s]", TTF_FONT_FILENAME, TTF_GetError());
    return 1;
  }

  _pText = TTF_RenderText_Solid(_pFont, label, blackColor);
  if (_pText == NULL) {
    log_error();
    return 1;
  }

  offset.x = (_pButton->w / 2) - (_pText->w / 2);
  offset.y = (_pButton->h / 2) - (_pText->h / 2);
  SDL_BlitSurface(_pText, NULL, _pButton, &offset);

  offset.x = (pMenu->w / 2) - (_pButton->w / 2);
  offset.y = (pMenu->h / 2) - (_pButton->h / 2);
  SDL_BlitSurface(_pButton, NULL, pMenu, &offset);

  TTF_CloseFont(_pFont);
  return 0;
}

// vim: ts=2:sw=2:expandtab
