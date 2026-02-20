main: main.o tetris.o
	gcc -o main main.o tetris.o -lncurses

main.o: main.c tetris.h
	gcc -c -Wall main.c

tetris.o: tetris.c tetris.h
	gcc -c -Wall tetris.c

clean:
	rm main main.o tetris.o
