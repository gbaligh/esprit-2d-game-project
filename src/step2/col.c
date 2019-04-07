#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_ttf.h>
#include <math.h>

struct esprit_game_col_s {
	int type;
	SDL_Rect* objs[100]; /* TODO: Replace array with dynamic list */
} pCtx[1] = {{0}};

static int esprit_game_col_trigo(SDL_Rect *obja, SDL_Rect *objb)
{
	int ra,rb,xb,xa,ya,yb,distance;

	ra = (obja->w)/2;
	xa = (obja->x + (obja->w/2));
	ya = (obja->y + (obja->h/2));

	rb = (objb->w)/2;
	xb = (objb->x + (objb->w/2));
	yb = (objb->y + (objb->h/2));

	distance = sqrt(pow(xb - xa, 2) + pow(yb - ya, 2));

	return (distance <= (ra + rb)) ? 1 : 0;
}

int esprit_game_col_init(int type)
{
	pCtx->type = type;

	return 0;
}

int esprit_game_col_add(SDL_Rect *obj)
{
	int i = 0;
	for (i = 0; i < 100; ++i) {
		if (pCtx->objs[i] == NULL) pCtx->objs[i] = obj;
		return 0;
	}
	return 1;
}

int esprit_game_col_check(SDL_Rect *obj)
{
	int i = 0;
	for (i = 0; i < 100; ++i) {
		if (pCtx->objs[i] && esprit_game_col_trigo(obj, pCtx->objs[i]))
			return 1;
	}
	return 0;
}
