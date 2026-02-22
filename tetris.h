#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define ROWS 20
#define COLS 10

typedef struct tetris_state *Tetris;

Tetris create_game(void);
void tick(Tetris state);
void render_board(Tetris state, void drawch(int x, int y, char c));

void mvleft(Tetris state);
void mvright(Tetris state);
void rotcw(Tetris state);
void rotccw(Tetris state);
void mvdrop(Tetris state);

char getcell(Tetris state, int x, int y); // TO BE IMPLEMENTED

#endif
