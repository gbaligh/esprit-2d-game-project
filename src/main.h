#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE(_p_) (sizeof(_p_)/sizeof(_p_[0]))

struct main_obj_s {
  char *name;
  int (*obj_event_handler)(SDL_Event *pEvent);
  int (*obj_blit)(SDL_Surface *pDst);
};

int main_register_obj(struct main_obj_s *pObj);

int main_isRunning(void);

void main_loop_stop(void);

int main_unregister_obj(struct main_obj_s *pObj);

#endif /* __MAIN_H__ */
// vim: ts=2:sw=2:expandtab
