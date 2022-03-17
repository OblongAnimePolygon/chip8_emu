#ifndef __DISPLAY_H
#define __DISPLAY_H
#include <SDL.h>
#include "display.h"
#include <stdbool.h>

typedef struct display
{
  SDL_Window *window;
  SDL_Surface *surface;
} display_t;

int display_init(display_t *d);
void update_display(display_t *d, bool bw_grid[64][32]);
void update_grid(display_t *d, uint32_t x, uint32_t   y, bool is_white);
void display_deinit(display_t *d);

#endif
