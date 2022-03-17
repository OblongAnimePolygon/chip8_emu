#include "readfile.h"
#include "log.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>

void filehandle_init(filehandle_t *f, const char *f_name)
{
  f->f_name = f_name;
  f->buf = NULL;
  f->buf_size = 0;
}
void filehandle_read(filehandle_t *f)
{
    struct stat st;
    size_t filesize = 0;
    FILE *fptr;

    stat(f->f_name, &st);
    filesize = st.st_size;

    f->buf = malloc(filesize);
    f->buf_size = filesize;


    fptr = fopen(f->f_name, "rb");
    fread(f->buf, f->buf_size, 1, fptr);
    fclose(fptr);

}
void filehandle_deinit(filehandle_t *f)
{
  free(f->buf);
  f->buf = NULL;
  f->buf_size = 0;
}
