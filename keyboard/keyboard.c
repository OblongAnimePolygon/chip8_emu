#include "keyboard_map.h"
#include "keyboard.h"
#include "log.h"
#include <assert.h>

static uint8_t scancode_map[16] = {
  SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
  SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R,
  SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
  SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V
};

static uint8_t keycode_to_index[16] = 
{
  13 /*0*/, 0 /*1*/, 1 /*2*/, 2 /*3*/, 
  4 /*4*/, 5 /*5*/, 6 /*6*/, 8 /*7*/,
  9 /*8*/, 10 /*9*/, 12 /*A*/, 14 /*B*/, 
  3 /*C*/, 7 /*D*/, 11 /*E*/, 15 /*F*/
};

void update_keyboard_state(bool *keyboard_state)
{
  SDL_Event e;
  uint8_t i;
  const uint8_t *cur_keystates;

  //handle all events in queue
  while (SDL_PollEvent(&e) != 0)
  {
    if (e.type == SDL_QUIT)
    {
      LOG("Recieved quit signal from user");
      assert(false);
    }
  }

  cur_keystates = SDL_GetKeyboardState(NULL);

  for (i = 0; i < 16; i++)
  {
    keyboard_state[i] = (bool) cur_keystates[scancode_map[i]];
  }

}

uint8_t key_to_index(uint8_t key)
{
  return keycode_to_index[key];
}
