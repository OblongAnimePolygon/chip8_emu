#include "log.h"
#include <stdint.h>

void hexdump(void *buf, size_t bufsize)
{
  size_t i;
  uint8_t *byte_buf = (uint8_t *) buf;
  printf("0x");
  for (i = 0; i < bufsize; i++)
  {
    printf("%02X", byte_buf[i]);
  }
}
