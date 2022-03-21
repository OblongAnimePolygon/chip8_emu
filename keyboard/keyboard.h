#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <stdbool.h>

void update_keyboard_state(bool *keyboard_state);
uint8_t key_to_index(uint8_t key);

#endif
