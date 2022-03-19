#ifndef __CHIP8_H
#define __CHIP8_H

#include <stdint.h>
#include <stdbool.h>
#include "display.h"

#define CHIP8_RAM_SIZE 0x400000 //Ram size is
#define DISPLAY_W 64
#define DISPLAY_H 32

typedef struct chip8_io
{
  display_t *display;
} chip8_io_t;

typedef struct chip8
{
  uint8_t RAM[CHIP8_RAM_SIZE];
  bool display[DISPLAY_W][DISPLAY_H];
  bool keyboard_state[16];
  uint16_t PC;
  uint64_t l;
  uint16_t stack_ptr;
  uint8_t delay_timer;
  uint8_t sound_timer;
  uint8_t V[16];
  chip8_io_t io;
} chip8_t;

#endif
