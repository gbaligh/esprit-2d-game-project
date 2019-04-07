#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include "col.h"
#include "hero.h"

struct esprit_game_obj_s {
	SDL_Rect pos;
	SDL_Surface *img;
};

struct esprit_game_ctx_s {
	SDL_version ver;
	SDL_Event ev;
	SDL_Surface *screen;
	SDL_Surface *bg;
	struct esprit_game_obj_s objs[];
};

static struct esprit_game_cfg_s {
	const char *img_path;
	const char *bg_file;
} app_cfg = {
	.img_path = "img",
	.bg_file  = "image.png"
};

int esprit_game_init(struct esprit_game_ctx_s *pCtx)
{
	SDL_VERSION(&pCtx->ver);
	printf("Compiled against SDL %d.%d.%d\n", pCtx->ver.major, pCtx->ver.minor, pCtx->ver.patch);

	/* Init SDL */
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Could not initialize SDL");
		return 1;
	}

	srand(SDL_GetTicks());

	/* Init IMG for PNG loading */
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		fprintf(stderr, "Failed to init SDL_image - PNG - %s\n", IMG_GetError());
		goto ERROR_L1;
	}

	/* Init TTF */
	if (TTF_Init() < 0) {
		fprintf(stderr, "Error initializing TTF: %s\n", TTF_GetError());
		goto ERROR_L2;
	}

	/* Init Audio */
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0) {
		fprintf(stderr, "Error OpenAudio: %s\n", Mix_GetError());
	}

	/* Screen resolution */
	pCtx->screen = SDL_SetVideoMode(1600, 400, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if(!pCtx->screen) { 
		fprintf(stderr, "Unable to set video mode: %s\n",SDL_GetError());
		goto ERROR_L3;
	}

	/* Background image */
	pCtx->bg = IMG_Load(app_cfg.bg_file); 
	if(!pCtx->bg) {
		fprintf(stderr, "Unable to load the image: %s\n",SDL_GetError());
		goto ERROR_L3;
	}

	SDL_WM_SetCaption("Mon premier jeux",NULL);

	if (esprit_game_col_init(0) != 0) {
		fprintf(stderr, "Error initializing collizing engine\n");
		goto ERROR_L3;
	}

	if (esprit_game_hero_init() != 0) {
		fprintf(stderr, "Error initializing hero\n");
		goto ERROR_L3;
	}

	/* Show screen */
	SDL_BlitSurface(pCtx->bg, NULL, pCtx->screen, NULL);

	return 0;

ERROR_L3:
	TTF_Quit();
ERROR_L2:
	IMG_Quit();
ERROR_L1:
	SDL_Quit();
	return 1;
}

int main(void)
{
	struct esprit_game_ctx_s ctx = {{0}};

	if (esprit_game_init(&ctx) != 0) {
		fprintf(stderr, "Error initializing game\n");
		return 255;
	}

	/* Create one obstact and add it into map for 
	 * collision detection */
	SDL_Rect obs = {
		.x = 660,
		.y = 250,
		.w = 160,
		.h = 30
	};
	if (esprit_game_col_add(&obs) != 0) {
		fprintf(stderr, "Error adding object\n");
		return 255;
	}

	int quit = 0;
	while(!quit)
	{
		SDL_PollEvent(&ctx.ev);

		if (ctx.ev.type == SDL_QUIT) {
			quit = 1;
			break;
		}
		SDL_BlitSurface(ctx.bg, NULL, ctx.screen, NULL);
		
		esprit_game_hero_ev(&ctx.ev);
		esprit_game_hero_show(ctx.screen);

		SDL_Flip(ctx.screen);
	}

	SDL_FreeSurface(ctx.bg);

	SDL_Quit;

	return 0;
}
