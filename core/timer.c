#include <chip8.h>
#include <sys/time.h>
#include <unistd.h>
#include "log.h"
#include "ops.h"
#include "display.h"

#define REFRESH_PER_SEC      60
#define INSTRUCTIONS_PER_SEC 749 //This should be 750 but for whatever reason execution loop runs slow

#define DEBUG_TIME_IN_SEC    4

#define CALCULATE_RATES      true

#define CLOCK_CYCLE_IN_US       (1000 * 1000) / REFRESH_PER_SEC
#define INSTRUCTION_CYCLE_IN_US (1000 * 1000) / INSTRUCTIONS_PER_SEC

#define DEBUG_EXEC_TICKS INSTRUCTIONS_PER_SEC * DEBUG_TIME_IN_SEC
#define DEBUG_REFRESH_TICKS REFRESH_PER_SEC * DEBUG_TIME_IN_SEC

#define TIME_DIFF_US(start, end) ((end)->tv_sec - (start)->tv_sec) * (1000 * 1000) + \
                                 ((end)->tv_usec - (start)->tv_usec)

static int ticks;
static int display_ticks;

static struct timeval display_start, exec_start;

// Handles timers, as well as updating the display
static void chip8_tick(chip8_t *emu)
{
  if (emu->delay_timer)
  {
    emu->delay_timer--;
  }

  if (emu->sound_timer)
  {
    emu->sound_timer--;
  }

  //update IO
  update_display((emu->io).display, emu->display);

  return;
}

static float calculate_rate(struct timeval *start, struct timeval *end, int ticks)
{
  return (((float) ticks * (1000 * 1000))) / ((float) TIME_DIFF_US(start, end)) ;
}

void chip8_timer(chip8_t *emu)
{
  struct timeval start, end;
  uint64_t useconds;

  gettimeofday(&display_start, NULL);

  display_ticks =  0;
  while(1)
  {
    gettimeofday(&start, NULL);
    chip8_tick(emu);
    gettimeofday(&end, NULL);

    useconds = TIME_DIFF_US(&start, &end);

    if (useconds < CLOCK_CYCLE_IN_US) usleep(CLOCK_CYCLE_IN_US - useconds);

    display_ticks++;
    if (display_ticks > DEBUG_REFRESH_TICKS) break;

    if (CALCULATE_RATES)
    {
      gettimeofday(&end, NULL);
      LOG("Screen FPS: %.2f", calculate_rate(&display_start, &end, display_ticks));
      LOG("Exec IPS: %.2f", calculate_rate(&exec_start, &end, ticks), ticks);
    }

  }

}

void chip8_instruction_timer(chip8_t *emu)
{
  struct timeval start, end;
  uint64_t useconds;
  uint16_t instruction = 0;

  gettimeofday(&exec_start, NULL);
  ticks = 0;

  while(1)
  {
    gettimeofday(&start, NULL);

    instruction = emu->RAM[emu->PC];
    instruction = (instruction << 8) | emu->RAM[emu->PC + 1];

    chip8_execute(emu, instruction);

    gettimeofday(&end, NULL);

    useconds = TIME_DIFF_US(&start, &end);
    if (useconds < INSTRUCTION_CYCLE_IN_US) usleep(INSTRUCTION_CYCLE_IN_US - useconds);

    //for debugging
    ticks++;
    if (ticks > DEBUG_EXEC_TICKS) break;

  }
}
