#ifndef __MENU_H__
#define __MENU_H__

int menu_init(void);

int menu_deinit(void);

int menu_action(SDL_Event *pEvent);

int menu_blit(SDL_Surface *pDst);

#endif /* __MENU_H__ */
