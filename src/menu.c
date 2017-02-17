#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>

#define SCREEN_WIDTH  357
#define SCREEN_HEIGHT 280
#define SCREEN_BPP 32

#define TTF_FONT_FILENAME "Roboto-Italic.ttf"
#define TTF_FONT_SIZE 64

#define IMG_BACKGROUND_FILENAME "background.png"
#define IMG_BTN1_FILENAME "play.png"
#define IMG_BTN2_FILENAME "multiplayer.png"
#define IMG_BTN3_FILENAME "option.png"
#define IMG_BTN4_FILENAME "quit.png" 

#define MIX_EF_FILENAME "switch.wav"
#define MIX_BACK_FILENAME "back.mp3"

typedef struct button_str {
  const char *img;
  SDL_Surface *but;
  SDL_Rect pos;
  SDL_Rect loc;
} button_t;

static button_t gButtons[] = {
  {"play.png",        NULL, {0}, {110,70,140,30}},
  {"multiplayer.png", NULL, {0}, {110,115,140,30}},
  {"option.png",      NULL, {0}, {110,160,140,30}},
  {"quit.png",        NULL, {0}, {110,205,140,30}}
};

int main(int argc, char *argv[])
{
  int quit = 0;

  SDL_Surface *ecran = NULL;
  SDL_Surface *menu = NULL;
  SDL_Surface *texte = NULL;
  SDL_Rect position_menu;
  SDL_Rect position_texte;
  SDL_Event event;
  TTF_Font *police = NULL;
  Mix_Music *musique = NULL;
  Mix_Chunk *musique1 = NULL; 
  int _i = 0;
  int num_but = sizeof(gButtons)/sizeof(gButtons[0]);
  int selected_but = num_but - 1;


  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return 255;
  }

  if (TTF_Init() != 0) {
    fprintf(stderr, "Error initializing TTF: %s\n", TTF_GetError());
    SDL_Quit();
    return 255;
  }

  ecran = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_HWSURFACE|SDL_DOUBLEBUF);
  if (ecran == NULL) {
    fprintf(stderr, "Error creating main surface: %s\n", SDL_GetError());
    goto Exit;
  }

  police = TTF_OpenFont(TTF_FONT_FILENAME, TTF_FONT_SIZE);
  if (police == NULL) {
    fprintf(stderr, "Error opening font file: %s\n", TTF_GetError());
    goto Exit;
  }

  TTF_SetFontStyle(police, TTF_STYLE_ITALIC);

  menu = IMG_Load(IMG_BACKGROUND_FILENAME);
  if (menu == NULL) {
    fprintf(stderr, "Error Loading image %s: %s\n", IMG_BACKGROUND_FILENAME, IMG_GetError());
    goto Exit;
  }

  for (_i = 0; _i < num_but; ++_i) {
    fprintf(stdout, "Loading %s\n", gButtons[_i].img);
    gButtons[_i].but = IMG_Load(gButtons[_i].img);
    if (gButtons[_i].but == NULL) {
      fprintf(stderr, "Error Loading image %s: %s\n", gButtons[_i].img, IMG_GetError());
      goto Exit;
    }
  }
  
  position_texte.x = 0;
  position_texte.y = 20;

  if (SDL_BlitSurface(menu, NULL, ecran, &position_menu) != 0) {
    fprintf(stderr, "Error draw menu image: %s\n", SDL_GetError());
    goto Exit;
  }

  for (_i = 0; _i < num_but; ++_i) {
    if (SDL_BlitSurface(gButtons[_i].but, NULL, ecran, &gButtons[_i].pos) != 0) {
      fprintf(stderr, "Error draw %s image: %s\n", gButtons[_i].img, SDL_GetError());
      goto Exit;
    }
  }

  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) {
    fprintf(stderr, "Error OpenAudio: %s\n", Mix_GetError());
    //goto Exit;
  }

  musique = Mix_LoadMUS(MIX_BACK_FILENAME);
  if (musique == NULL) {
    fprintf(stderr, "Error in loading background music: %s\n", Mix_GetError());
    //goto Exit;
  }

  /* Play music for the menu */ 
  if (Mix_PlayMusic(musique, -1) != 0) {
    fprintf(stderr, "Error playing music: %s\n", Mix_GetError());
    //goto Exit;
  }

  /* Load button switching sound effect */
  musique1 = Mix_LoadWAV(MIX_EF_FILENAME); 
  if (musique1 == NULL) {
    fprintf(stderr, "Error Mix_LoadWAV: %s\n", Mix_GetError());
    //goto Exit;
  }

  while (!quit) {
    SDL_WaitEvent(&event);
    switch (event.type) {
      case SDL_QUIT :
        quit = 1;
        break;
      case SDL_MOUSEMOTION:
        fprintf(stdout, "Mouse(x,y)=(%d,%d)\n", event.motion.x, event.motion.y);
        for (_i = 0; _i < num_but; ++_i) {
          if ((event.motion.x > gButtons[_i].loc.x ) && (event.motion.x < gButtons[_i].loc.x + gButtons[_i].loc.w ) && (event.motion.y > gButtons[_i].loc.y ) && (event.motion.y < gButtons[_i].loc.y + gButtons[_i].loc.h )) {
            selected_but = _i;
            Mix_PlayChannel( -1, musique1, 0 );
            SDL_BlitSurface(gButtons[selected_but].but, NULL, ecran, &gButtons[selected_but].pos);
            fprintf(stdout, "Mouse position match %d: %d,%d,%d,%d\n", _i, gButtons[_i].pos.x, gButtons[_i].pos.y, gButtons[_i].pos.w, gButtons[_i].pos.h);
            break;
          }
        }
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
          case SDLK_DOWN:
            {
              Mix_PlayChannel( -1, musique1, 0 );
              selected_but = ++selected_but % num_but;
              SDL_BlitSurface(gButtons[selected_but].but, NULL, ecran, &gButtons[selected_but].pos);
              break;
            }

          case SDLK_UP:
            {
              Mix_PlayChannel( -1, musique1, 0 );
              --selected_but;
              if (selected_but < 0) selected_but = num_but - 1;
              SDL_BlitSurface(gButtons[selected_but].but, NULL, ecran, &gButtons[selected_but].pos);
              break;
            }
        }
    }
    SDL_BlitSurface(texte, NULL, ecran, &position_texte);
    SDL_Flip(ecran);
  }

Exit:
  Mix_FreeMusic(musique); 
  Mix_CloseAudio();
  TTF_CloseFont(police);
  TTF_Quit();
  SDL_Quit();
  return EXIT_SUCCESS;
}

// vim: ts=2:sw=2:expandtab
