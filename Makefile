CC=gcc
CFLAGS= `sdl2-config --cflags --libs` -pthread
INCLUDES=
LIBS= `sdl2-config --cflags --libs` -pthread

C_SRCS=
C_OBJS=

CORE=core
LOG=log
DISPLAY=display

READFILE=readfile

ODIR=obj

#add log library
C_SRCS+= $(notdir $(wildcard $(LOG)/*.c))
INCLUDES+=$(LOG)
vpath %.c $(LOG)

#add core library
C_SRCS+= $(notdir $(wildcard $(CORE)/*.c))
INCLUDES+=$(CORE)
vpath %.c $(CORE)

#add readfile library
C_SRCS+= $(notdir $(wildcard $(READFILE)/*.c))
INCLUDES+=$(READFILE)
vpath %.c $(READFILE)

#add display library
C_SRCS+= $(notdir $(wildcard $(DISPLAY)/*.c))
INCLUDES+=$(DISPLAY)
vpath %.c $(DISPLAY)


CFLAGS+=$(addprefix -I, $(INCLUDES))
C_OBJS+= $(patsubst %.c, %.o, $(C_SRCS))
C_OBJS_DIR := $(patsubst %,$(ODIR)/%,$(C_OBJS))

$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

chip8: $(C_OBJS_DIR)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o chip8
