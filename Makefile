CC      = gcc
CFLAGS  = -ansi -pedantic -Wall -Wextra \
           $(shell sdl-config --cflags)
LDFLAGS = $(shell sdl-config --libs)

SRCS    = Demo.c DigiFont.c
OBJS    = $(SRCS:.c=.o)
TARGET  = digifont_demo

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean