#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define ROWS 20
#define COLS 10

extern char board[ROWS][COLS];

void tick();
void start_game();

void mvleft(void);
void mvright(void);
void rotcw(void);
void rotccw(void);
void mvdrop(void);

#endif
