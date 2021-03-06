#ifndef __KEYBOARD_MAP_H
#define __KEYBOARD_MAP_H

#include <SDL.h>
/**
Chip 8 keyboard layout

1	2	3	C
4	5	6	D
7	8	9	E
A	0	B	F

1(0) 2(1) 3(2) C(3)
4(4) 5(5) 6(6) D(7)
7(8) 8(9) 9(10) E(11)
A(12) 0(13) B(14) F(15)

US Keyboard layout

1 2 3 4
Q W E R
A S D F
Z X C V

*/
#define CHIP8_1_KEY_MAP  SDL_SCANCODE_1
#define CHIP8_1_KEY_INDEX 0

#define CHIP8_2_KEY_MAP  SDL_SCANCODE_2
#define CHIP8_2_KEY_INDEX 1

#define CHIP8_3_KEY_MAP SDL_SCANCODE_3
#define CHIP8_3_KEY_INDEX 2

#define CHIP8_C_KEY_MAP SDL_SCANCODE_4
#define CHIP8_C_KEY_INDEX 3

#define CHIP8_4_KEY_MAP SDL_SCANCODE_Q
#define CHIP8_4_KEY_INDEX 4

#define CHIP8_5_KEY_MAP SDL_SCANCODE_W
#define CHIP8_5_KEY_INDEX 5

#define CHIP8_6_KEY_MAP SDL_SCANCODE_E
#define CHIP8_6_KEY_INDEX 6

#define CHIP8_D_KEY_MAP SDL_SCANCODE_R
#define CHIP8_D_KEY_INDEX 7

#define CHIP8_7_KEY_MAP SDL_SCANCODE_A
#define CHIP8_7_KEY_INDEX 8

#define CHIP8_8_KEY_MAP SDL_SCANCODE_S
#define CHIP8_8_KEY_INDEX 9

#define CHIP8_9_KEY_MAP SDL_SCANCODE_D
#define CHIP8_9_KEY_INDEX 10

#define CHIP8_E_KEY_MAP SDL_SCANCODE_F
#define CHIP8_E_KEY_INDEX 11

#define CHIP8_A_KEY_MAP SDL_SCANCODE_Z
#define CHIP8_A_KEY_INDEX 12

#define CHIP8_0_KEY_MAP SDL_SCANCODE_X
#define CHIP8_0_KEY_INDEX 13

#define CHIP8_B_KEY_MAP SDL_SCANCODE_C
#define CHIP8_B_KEY_INDEX 14

#define CHIP8_F_KEY_MAP SDL_SCANCODE_V
#define CHIP8_F_KEY_INDEX 15


#endif
