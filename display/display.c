#include <SDL.h>
#include <stdio.h>
#include "display.h"
#include "log.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320
#define SQUARE_SIDE_LEN 10


static SDL_Rect newSDL_Rect(int xs, int ys, int widths, int heights)
{
    SDL_Rect rectangular;
    rectangular.x = xs;
    rectangular.y = ys;
    rectangular.w = widths;
    rectangular.h = heights;
    return rectangular;
}

int display_init(display_t *d)
{
  d->window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              WINDOW_WIDTH, WINDOW_HEIGHT,
                              SDL_WINDOW_SHOWN);
  if (!d->window)
  {
    LOG("ERROR: UNABLE TO CREATE WINDOW!");
    return -1;
  }

  d->surface = SDL_GetWindowSurface(d->window);

  if (!d->surface)
  {
    LOG("ERROR: UNABLE TO CREATE SURFACE!");
    return -1;
  }

  //Fill the surface black
  SDL_FillRect( d->surface, NULL, SDL_MapRGB( d->surface->format, 0x00, 0x00, 0x00 ) );

  //Update the surface
  SDL_UpdateWindowSurface( d->window );
  SDL_Delay(10); //Need this for screen refresh lol

  return 0;
}

void update_display(display_t *d, bool bw_grid[64][32])
{
  SDL_Rect cur;
  DEBUGLOG("update display call");
  SDL_UpdateWindowSurface( d->window );
  SDL_Delay(3);
}

void update_grid(display_t *d, uint32_t x, uint32_t y, bool is_white)
{
  SDL_Rect cur = newSDL_Rect(x * SQUARE_SIDE_LEN, y * SQUARE_SIDE_LEN,
                    SQUARE_SIDE_LEN, SQUARE_SIDE_LEN);
  uint8_t color = 0x00;
  if (is_white)
  {
    color = 0xFF;
  }
  SDL_FillRect( d->surface, &cur,
                SDL_MapRGB( d->surface->format, color, color, color ) );
}

void clear_display(display_t *d)
{
  //Fill the surface black
  SDL_FillRect( d->surface, NULL, SDL_MapRGB( d->surface->format, 0x00, 0x00, 0x00 ) );

  //Update the surface
  SDL_UpdateWindowSurface( d->window );
  SDL_Delay(10); //Need this for screen refresh lol  
}

void display_deinit(display_t *d)
{
  SDL_DestroyWindow(d->window);
  SDL_Quit(); //remove this once more sdl dependencies
}
