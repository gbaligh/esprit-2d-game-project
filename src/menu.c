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
#define TTF_FONT_SIZE 58

#define IMG_BACKGROUND_FILENAME "img/background.png"
#define IMG_BUTTON_FILENAME "img/button.png"

struct button_s {
  const char *label;
  SDL_Surface *s;
  SDL_Rect p;
  int (*button_action)(void);
};

struct page_s {
  SDL_Surface *s;
};

struct menu_s {
  SDL_Surface *bg;
  SDL_Surface *button;
  TTF_Font *font;
  int idxButton;
};

extern int gameRunning;

static struct menu_s gMenu = {
  NULL,
  NULL,
  NULL,
  0,
};

static int play_action(void)
{
  log_info("Play game");
  return 0;
}

static int options_action(void)
{
  log_info("Option page to load ...");
  return 0;
}

static int quit_action(void)
{
  log_info("QUIT GAME: BYE BYE");
  gameRunning = 0;
  return 0;
}

static struct button_s gButtons[] = {
  {"play", NULL, {0}, play_action},
  {"multiplayer", NULL, {0}, NULL},
  {"options", NULL, {0}, options_action},
  {"quit", NULL, {0}, quit_action},
};

int Menu_Init()
{
  gMenu.bg = IMG_Load(IMG_BACKGROUND_FILENAME);
  if (gMenu.bg == NULL) {
    log_error("Loading background: %s", IMG_GetError());
    return 1;
  }

  gMenu.button = IMG_Load(IMG_BUTTON_FILENAME);
  if (gMenu.button == NULL) {
    log_error("Loading button: %s", IMG_GetError());
    SDL_FreeSurface(gMenu.bg);
    gMenu.bg = NULL;
    return 1;
  }

  gMenu.font = TTF_OpenFont(TTF_FONT_FILENAME, TTF_FONT_SIZE);
  if (gMenu.font == NULL) {
    SDL_FreeSurface(gMenu.bg);
    gMenu.bg = NULL;
    log_error("Loading font file(%s):[%s]", TTF_FONT_FILENAME, TTF_GetError());
    return 1;
  }

  return 0;
}

int Menu_Deinit()
{
  int _i = 0;
  int buttonNbr = sizeof(gButtons)/sizeof(gButtons[0]);

  SDL_FreeSurface(gMenu.bg);
  SDL_FreeSurface(gMenu.button);
  TTF_CloseFont(gMenu.font);
  
  for (_i=0; _i<buttonNbr; ++_i) {
    SDL_FreeSurface(gButtons[_i].s);
  }

  return 0;
}

int Menu_Action(SDL_Event *pEvent)
{
  static int lastEventType = -1;
  int _i = 0;
  int buttonNbr = sizeof(gButtons)/sizeof(gButtons[0]);

  /* keyboard handling */
  if ((pEvent->type == SDL_KEYDOWN) && (lastEventType != SDL_KEYDOWN)) {
    if (pEvent->key.keysym.sym == SDLK_DOWN) {
      gMenu.idxButton = ++gMenu.idxButton % buttonNbr;
    }
    if (pEvent->key.keysym.sym == SDLK_UP) {
      if (--gMenu.idxButton < 0) gMenu.idxButton += buttonNbr;
    }
    lastEventType = SDL_KEYDOWN;
  }
  if ((pEvent->type == SDL_KEYUP) && (lastEventType != SDL_KEYUP)) {
    lastEventType = SDL_KEYUP;
    /* Enter key: Action on button */
    if (pEvent->key.keysym.sym == SDLK_RETURN) {
      if (gButtons[gMenu.idxButton].button_action) {
        gButtons[gMenu.idxButton].button_action();
      }
    }
  }

  /* Mouse handling */
  if ((pEvent->type == SDL_MOUSEMOTION) || (pEvent->type == SDL_MOUSEBUTTONUP)) {
    for (_i = 0; _i < buttonNbr; ++_i) {
      if ((pEvent->motion.x > gButtons[_i].p.x ) && (pEvent->motion.x < gButtons[_i].p.x + gButtons[_i].p.w ) && (pEvent->motion.y > gButtons[_i].p.y ) && (pEvent->motion.y < gButtons[_i].p.y + gButtons[_i].p.h )) {
        gMenu.idxButton = _i;

        if (pEvent->motion.state == SDL_PRESSED) {
          if (gButtons[gMenu.idxButton].button_action) {
            gButtons[gMenu.idxButton].button_action();
          }
        }
      }
    }
  }

  return 0;
}

int Menu_Blit(SDL_Surface *pDst)
{
  if (SDL_BlitSurface(gMenu.bg, NULL, pDst, NULL) != 0) {
    log_error("Bliting background image: %s", SDL_GetError());
    return 1;
  }

  if (Menu_AddButtons(pDst) != 0) {
    log_error("Adding buttons to menu");
    return 1;
  }

  return 0;
}

int Menu_AddButtons(SDL_Surface *pDst)
{
  SDL_Surface *_pText = NULL;
  SDL_Color blackColor = {0};
  SDL_Color selectedColor = {255, 255, 128, 0};
  int _i = 0;
  int buttonNbr = sizeof(gButtons)/sizeof(gButtons[0]);
  SDL_Rect offset;

  if (gMenu.button == NULL) {
    log_error("Menu not initialized");
    return 1;
  }
  
  for(_i=0; _i<buttonNbr; ++_i) {
    /* Free old surface */
    if (gButtons[_i].s != NULL) {
      SDL_FreeSurface(gButtons[_i].s);
      gButtons[_i].s = NULL;
    }
    /* Copy new surface */
    gButtons[_i].s = SDL_ConvertSurface(gMenu.button, gMenu.button->format, SDL_HWSURFACE);
    if (gButtons[_i].s == NULL) {
      log_error("Copy the loaded image: %s", SDL_GetError());
      return 1;
    }
    /* render label */
    _pText = TTF_RenderText_Solid(gMenu.font, gButtons[_i].label, (gMenu.idxButton == _i) ? selectedColor : blackColor);
    if (_pText == NULL) {
      log_error("Font loading: %s", TTF_GetError());
      SDL_FreeSurface(gButtons[_i].s);
      return 1;
    }
    /* Set Text in the correct postion */
    offset.x = (gButtons[_i].s->w / 2) - (_pText->w / 2);
    offset.y = (gButtons[_i].s->h / 2) - (_pText->h / 2);
    SDL_BlitSurface(_pText, NULL, gButtons[_i].s, &offset);
  }

  /* offset of all buttons (center) */
  offset.x = (pDst->w / 2) - (gButtons[0].s->w / 2);
  int _y = (pDst->h / 2) - ((gButtons[0].s->h + gButtons[0].s->h/4) * (buttonNbr-1) / 2);

  for (_i=0; _i<buttonNbr; ++_i) {
    gButtons[_i].p.x = offset.x;
    gButtons[_i].p.y = _y + (gButtons[_i].s->h + gButtons[_i].s->h/4) * _i;
    SDL_BlitSurface(gButtons[_i].s, NULL, pDst, &gButtons[_i].p);
  }

  return 0;
}

// vim: ts=2:sw=2:expandtab
