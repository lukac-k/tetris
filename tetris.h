#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define ROWS 20
#define COLS 10
#define BLOCK_LEN 4

typedef struct tetris_state *Tetris;

Tetris create_game(void);
void destroy_game(Tetris state);
void tick(Tetris state);
void render_board(Tetris state, void *ctx, void drawch(void *ctx, int x, int y, char c));
void render_next(Tetris state, void *ctx, void drawch(void *ctx, int x, int y, char c));
int get_score(Tetris state);

void mvleft(Tetris state);
void mvright(Tetris state);
void rotcw(Tetris state);
void rotccw(Tetris state);
void mvdrop(Tetris state);

#endif
