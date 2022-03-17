#ifndef __READFILE_H
#define __READFILE_H

#include <stddef.h>

typedef struct file_handle
{
  const char *f_name;
  void *buf;
  size_t buf_size;
} filehandle_t;

void filehandle_init(filehandle_t *f, const char *f_name);
void filehandle_read(filehandle_t *f);
void filehandle_deinit(filehandle_t *f);

#endif
