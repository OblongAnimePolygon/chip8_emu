#include "ops.h"
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "log.h"
#include "display.h"
#include "keyboard.h"

#define CLEAR_SCREEN 0
#define JUMP 1
#define JUMP_TO_SUBROUTINE 2
#define SKIP_IF_EQ 3
#define SKIP_IF_NEQ 4
#define SKIP_IF_XY_EQ 5
#define SET_REGISTER 6
#define ADD_VALUE_REGISTER 7
#define MATH 8
#define SKIP_IF_XY_NEQ 9
#define SET_INDEX_REGISTER 10
#define JUMP_WITH_OFFSET 11
#define RANDOM           12
#define DISPLAY 13
#define SKIP_IF_KEY 14
#define TIMER  15

void draw(chip8_t *emu, uint8_t vx, uint8_t vy, uint8_t val)
{
  uint8_t start_x = vx % 64;
  uint8_t start_y = vy % 32;
  uint32_t i = 0;
  uint32_t j = 0;

  bool old = 0;

  uint8_t horizontal_byte = 0;

  emu->V[15] = 0;

  for (i = 0; i < val; i++)
  {
    if (start_y + i == 32) break; //sprite will clip
    horizontal_byte = emu->RAM[emu->l + i];

    for (j = 0; j < 8; j++)
    {
      if (start_x + j == 64) break; //sprite will clip
      old = emu->display[start_x + j][start_y + i];

      emu->display[start_x + j][start_y + i] ^=
        (bool) ((horizontal_byte >> (7 - j)) & 1);

      if (old && !emu->display[start_x + j][start_y + i])
      {
        emu->V[15] = 1;
      }

      if (old != emu->display[start_x + j][start_y + i])
      {
        update_grid((emu->io).display, start_x + j, start_y + i,
                     emu->display[start_x + j][start_y + i]);
      }
    }
  }

}

void skip_if_key(chip8_t *emu, uint8_t X, uint8_t NN)
{
  if (emu->V[X] > 15)
  {
    LOG("ERROR: SKIP INSTRUCTION VX REGISTER GREATER THAN 15");
    assert(false);
  }

  switch (NN)
  {
    case 0x9E:
      DEBUGLOG("SKIP IF KEYPRESSED, %u", emu->V[X]);
      if (emu->keyboard_state[key_to_index(emu->V[X])])
      {
        emu->PC += 2;
      }
      break;
    case 0xA1:
      DEBUGLOG("SKIP IF KEY NOT PRESSED, %u", emu->V[X]);
      if (!emu->keyboard_state[key_to_index(emu->V[X])])
      {
        emu->PC += 2;
      }
      break;
    default:
      LOG("ERROR: INVVALID SKIP_IF_KEY INSTRUCTION");
      assert(false);
  }
}

void math(chip8_t *emu, uint8_t X, uint8_t Y, uint8_t N)
{
  switch (N)
  {
    case 0:
      DEBUGLOG("SET");
      emu->V[X] = emu->V[Y];
      break;
    case 1:
      DEBUGLOG("OR");
      emu->V[X] |= emu->V[Y];
      break;
    case 2:
      DEBUGLOG("AND");
      emu->V[X] &= emu->V[Y];
      break;
    case 3:
      DEBUGLOG("XOR");
      emu->V[X] ^= emu->V[Y];
      break;
    case 4:
      DEBUGLOG("ADD");
      emu->V[15] = 0;
      if (emu->V[X] > (emu->V[X] + emu->V[Y]))
      {
        emu->V[15] = 1;
      }
      emu->V[X] = emu->V[X] + emu->V[Y];
      break;
    case 5:
      DEBUGLOG("SUBTRACT XY");
      emu->V[15] = 1;
      if (emu->V[Y] > emu->V[X])
      {
        emu->V[15] = 0;
      }
      emu->V[X] = emu->V[X] - emu->V[Y];
      break;
    case 7:
      DEBUGLOG("SUBTRACT YX");
      emu->V[15] = 1;
      if (emu->V[X] > emu->V[Y])
      {
        emu->V[15] = 0;
      }
      emu->V[X] = emu->V[Y] - emu->V[X];
      break;
    case 6:
      DEBUGLOG("SHIFT RIGHT");
      //do new behavior ignoring Y
      emu->V[15] = emu->V[X] & 1;
      emu->V[X] >>= 1;
      break;
    case 14:
      DEBUGLOG("SHIFT LEFT");
      emu->V[15] = emu->V[X] >> 7;
      emu->V[X] <<= 1;
      break;
    default:
      LOG("ERROR: Invalid math operation");
      assert(false);
  }
}

void chip8_execute(chip8_t *emu, uint16_t instruction)
{
  uint8_t N = (instruction & 0xF);
  uint8_t Y = (instruction & 0xF0) >> 4;
  uint8_t X = (instruction & 0xF00) >> 8;
  uint8_t opcode = (instruction & 0xF000) >> 12;

  uint8_t NN  = (instruction & 0xFF);
  uint16_t NNN = (instruction & 0xFFF);

  emu->PC += 2;

  switch(opcode)
  {
    case CLEAR_SCREEN:
      if (NNN == 0xE0)
      {
        //Need to add machine shit lata?
        DEBUGLOG("CLEAR_SCREEN");
        memset(emu->display, 0, sizeof(emu->display));
        clear_display((emu->io).display);
      }
      else if (NNN == 0xEE) //return from subroutine
      {
        DEBUGLOG("RETURN FROM SUBROUTINE");
        emu->stack_ptr -= 2;
        emu->PC = emu->RAM[emu->stack_ptr];
        emu->PC = (emu->PC << 8) | emu->RAM[emu->stack_ptr + 1];
      }
      else
      {
        LOG("ERROR!: Not a NNN: %u", opcode);
        assert(false);
      }
      break;
    case JUMP:
      DEBUGLOG("JUMP %u", NNN);
      emu->PC = NNN;
      break;
    case JUMP_TO_SUBROUTINE:
      DEBUGLOG("JUMP_TO_SUBROUTINE %u", NNN);
      emu->RAM[emu->stack_ptr] = (instruction & 0xFF00) >> 8;
      emu->RAM[emu->stack_ptr + 1] = (instruction & 0xFF);
      
      emu->stack_ptr += 2;
      emu->PC = NNN;
      break;
    case SKIP_IF_EQ:
      DEBUGLOG("SKIP_IF_EQ");
      if (emu->V[X] == NN)
      {
        emu->PC += 2;
      }
      break;
    case SKIP_IF_NEQ:
      DEBUGLOG("SKIP_IF_NEQ");
      if (emu->V[X] != NN)
      {
        emu->PC += 2;
      }
      break;
    case SKIP_IF_XY_EQ:
      DEBUGLOG("SKIP_IF_XY_EQ");
      if (emu->V[X] == emu->V[Y])
      {
        emu->PC += 2;
      }
      break;
    case SET_REGISTER:
      DEBUGLOG("SET_REGISTER %u, %u", X, NN);
      emu->V[X] = NN;
      break;
    case ADD_VALUE_REGISTER:
      DEBUGLOG("ADD VALUE REGISTER, %u, %u", X, NN);
      emu->V[X] += NN;
      break;
    case MATH:
      math(emu, X, Y, N);
      break;
    case SKIP_IF_XY_NEQ:
      DEBUGLOG("SKIP_IF_XY_NEQ");
      if (emu->V[X] != emu->V[Y])
      {
        emu->PC += 2;
      }
      break;
    case SET_INDEX_REGISTER:
      DEBUGLOG("SET INDEX REGISTER, %u", NNN);
      emu->l = NNN;
      break;
    case JUMP_WITH_OFFSET:
      DEBUGLOG("JUMP WITH OFFSET, %u", NNN);
      emu->PC = NNN;
      emu->V[X] = NNN;
    case RANDOM:
      DEBUGLOG("RANDOM, %u, %u", X, NN);
      emu->V[X] = ((uint8_t) rand()) & NN;
    case DISPLAY:
      DEBUGLOG("DISPLAY, %u, %u, %u", X, Y, N);
      draw(emu, emu->V[X], emu->V[Y], N);
      break;
    case SKIP_IF_KEY:
      skip_if_key(emu, X, NN);
      break;
    default:
      LOG("ERROR!: Not a valid instruction: %u", opcode);
      assert(false);
  }

}
