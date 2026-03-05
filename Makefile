SHELL = /bin/sh
CC ?= cc
CFLAGS = -Wall
LDLIBS = -lncurses

objects = main.o tetris.o

main: $(objects)
	$(CC) -o main $(objects) $(LDLIBS)

main.o: main.c tetris.h
	$(CC) -c $(CFLAGS) main.c

tetris.o: tetris.c tetris.h
	$(CC) -c $(CFLAGS) tetris.c

.PHONY: clean
clean:
	rm main $(objects)
