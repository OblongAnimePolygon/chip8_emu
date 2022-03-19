#include "ops.h"
#include <string.h>
#include <assert.h>
#include "log.h"
#include "display.h"

#define CLEAR_SCREEN 0
#define JUMP 1
#define SET_REGISTER 6
#define ADD_VALUE_REGISTER 7
#define SET_INDEX_REGISTER 10
#define DISPLAY 13

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
    case SET_REGISTER:
      DEBUGLOG("SET_REGISTER %u, %u", X, NN);
      emu->V[X] = NN;
      break;
    case ADD_VALUE_REGISTER:
      DEBUGLOG("ADD VALUE REGISTER, %u, %u", X, NN);
      emu->V[X] += NN;
      break;
    case SET_INDEX_REGISTER:
      DEBUGLOG("SET INDEX REGISTER, %u", NNN);
      emu->l = NNN;
      break;
    case DISPLAY:
      DEBUGLOG("DISPLAY, %u, %u, %u", X, Y, N);
      draw(emu, emu->V[X], emu->V[Y], N);
      break;
    default:
      LOG("ERROR!: Not a valid instruction: %u", opcode);
      assert(false);
  }

}
