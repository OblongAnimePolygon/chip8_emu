#include "chip8.h"
#include "fonts.h"
#include "timer.h"
#include "log.h"
#include "readfile.h"
#include "display.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>

static void *chip_timer_wrapper(void *args)
{
  chip8_timer((chip8_t *) args);
  return NULL;
}

static void *execution_timer_wrapper(void *args)
{
  chip8_instruction_timer((chip8_t *) args);
  return NULL;
}

void chip_init(chip8_t *emu, const char *rom_name, display_t *d)
{
  filehandle_t rom_fh;
  memset(&emu->RAM, 0, sizeof(emu->RAM));
  //Initializing font data
  memcpy(&emu->RAM[0x50], FONT_DATA, sizeof(FONT_DATA));

  //Copy over the ROM
  filehandle_init(&rom_fh, rom_name);
  filehandle_read(&rom_fh);
  memcpy(&emu->RAM[0x200], rom_fh.buf, rom_fh.buf_size);
  filehandle_deinit(&rom_fh);

  //Initialize timers
  emu->delay_timer = 0;
  emu->sound_timer = 0;

  //Initialize all registers
  emu->PC=0x200;
  emu->l = 0;
  emu->stack_ptr = 0;
  memset(emu->V, 0, sizeof(emu->V));

  //set I/O handles
  (emu->io).display = d;
}

int main(int argc, char *argv[])
{

  chip8_t emu;
  display_t d;
  filehandle_t filehandle;
  pthread_t display_thread, execution_thread;

  if (argc < 2)
  {
    LOG("ERROR: No rom file provided");
    return -1;
  }

  display_init(&d);
  chip_init(&emu, argv[1], &d);

  pthread_create(&execution_thread, NULL, execution_timer_wrapper, &emu);
  chip8_timer(&emu); //what the fuck why can i not run this inside a thread????

  LOG("Waiting for execution thread to finish");
  pthread_join(execution_thread, NULL);
  LOG("Execution thread done");

  display_deinit(&d);

  return 0;
}
