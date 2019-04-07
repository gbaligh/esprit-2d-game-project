#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include "col.h"
#include "hero.h"

static struct esprit_game_hero_s {
	SDL_Surface *srf;
	SDL_Rect pos;
	const char *img;
	const int step;
} pCtx[1] = {
	{
		.img = "detective.png",
		.step = 10
	}
};

int esprit_game_hero_init()
{
	pCtx->srf = IMG_Load(pCtx->img);
	if (!pCtx->srf) {
		fprintf(stderr, "Unable to load the image: %s\n",SDL_GetError());
		return 1;
	}

	SDL_SetColorKey(pCtx->srf, SDL_SRCCOLORKEY, SDL_MapRGB(pCtx->srf->format, 255,255, 255));

	pCtx->pos.x = 300;
	pCtx->pos.y = 200;

	return 0;
}

int esprit_game_hero_show(SDL_Surface *screen)
{
	SDL_BlitSurface(pCtx->srf, NULL, screen, &pCtx->pos);
	return 0;
}

int esprit_game_hero_ev(SDL_Event *ev)
{
	switch(ev->type)
	{
		case SDL_KEYDOWN:
			if (ev->key.keysym.sym == SDLK_UP) {
				pCtx->pos.y -= pCtx->step;
				if (esprit_game_col_check(&pCtx->pos))
					pCtx->pos.y += pCtx->step;
			}
			if (ev->key.keysym.sym == SDLK_DOWN) {
				pCtx->pos.y += pCtx->step;
				if (esprit_game_col_check(&pCtx->pos))
					pCtx->pos.y -= pCtx->step;
			}
			if (ev->key.keysym.sym == SDLK_RIGHT) {
				pCtx->pos.x += pCtx->step;
				if (esprit_game_col_check(&pCtx->pos))
					pCtx->pos.x -= pCtx->step;
			}
			if (ev->key.keysym.sym == SDLK_LEFT) {
				pCtx->pos.x -= pCtx->step;
				if (esprit_game_col_check(&pCtx->pos))
					pCtx->pos.x += pCtx->step;
			}
			break;
	}
	return 0;
}
