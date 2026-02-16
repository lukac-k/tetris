tetris: tetris.o game.o
	gcc -o tetris tetris.o game.o -lncurses

tetris.o: tetris.c game.h
	gcc -c -Wall tetris.c

game.o: game.c game.h
	gcc -c -Wall game.c

clean:
	rm tetris tetris.o game.o
