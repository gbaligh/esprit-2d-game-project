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

#define TTF_FONT_FILENAME "ttf/Roboto-Italic.ttf"
#define TTF_FONT_SIZE 58

#define IMG_BACKGROUND_FILENAME "img/background.png"
#define IMG_BUTTON_FILENAME "img/button.png"

#define MIX_EF_FILENAME "mix/switch.wav"
#define MIX_BACK_FILENAME "mix/back.mp3"

struct button_s {
  /* Label of the button */
  const char *label;
  /* Original surface */
  SDL_Surface *s;
  /* Positon of the surface in the screen */
  SDL_Rect p;
  /* Action of the button */
  int (*button_action)(void);
};

struct page_s {
  SDL_Surface *s;
};

struct menu_s {
  /* Background image surface */
  SDL_Surface *bg;
  /* Button image surface */
  SDL_Surface *button;
  /* Font for TTF */
  TTF_Font *font;
  /* background music */
  Mix_Music *music;
  int isMusicPlaying;
  /* Effect sound */
  Mix_Chunk *effect;
  /* Selected button */
  int idxButton;
};

extern int gameRunning;

static struct menu_s gMenu;

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

  gMenu.music = Mix_LoadMUS(MIX_BACK_FILENAME);
  if (gMenu.music == NULL) {
    log_error("loading background music: %s\n", Mix_GetError());
  }

  gMenu.effect = Mix_LoadWAV(MIX_EF_FILENAME); 
  if (gMenu.effect == NULL) {
    log_error("Mix_LoadWAV: %s\n", Mix_GetError());
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
      gMenu.idxButton = (gMenu.idxButton + 1) % buttonNbr;
    }
    if (pEvent->key.keysym.sym == SDLK_UP) {
      if (--gMenu.idxButton < 0) gMenu.idxButton += buttonNbr;
    }
    lastEventType = SDL_KEYDOWN;
    Mix_PlayChannel(-1, gMenu.effect, 0);
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
      if ((pEvent->motion.x > gButtons[_i].p.x ) && 
          (pEvent->motion.x < gButtons[_i].p.x + gButtons[_i].p.w ) && 
          (pEvent->motion.y > gButtons[_i].p.y ) && 
          (pEvent->motion.y < gButtons[_i].p.y + gButtons[_i].p.h )) {
        /* Selected button */
        gMenu.idxButton = _i;
        Mix_PlayChannel(-1, gMenu.effect, 0);

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

  if (!gMenu.isMusicPlaying) { 
    gMenu.isMusicPlaying = !Mix_PlayMusic(gMenu.music, -1);
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
  
  for (_i=0; _i<buttonNbr; ++_i) {
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
  int _x = (pDst->w / 2) - (gMenu.button->w / 2);
  int _y = (pDst->h / 2) - ((gMenu.button->h + gMenu.button->h/4) * (buttonNbr-1) / 2);

  for (_i=0; _i<buttonNbr; ++_i) {
    gButtons[_i].p.x = _x;
    gButtons[_i].p.y = _y + (gButtons[_i].s->h + gButtons[_i].s->h/4) * _i;
    SDL_BlitSurface(gButtons[_i].s, NULL, pDst, &gButtons[_i].p);
  }

  return 0;
}

// vim: ts=2:sw=2:expandtab
