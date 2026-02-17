#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"

#define BLOCK_LEN 4
#define N_TETRIMINO 7
#define ORIENTATIONS 4

struct tetrimino {
	int orientation;
	const char (*block)[BLOCK_LEN][BLOCK_LEN];
	int posx;
	int posy;
};
typedef struct tetrimino Tetr;

char board[ROWS][COLS];
static Tetr *cur;
static Tetr *next;

static int curtick;
static int score;
static int gameover;

static const char TETRIMINO[N_TETRIMINO][ORIENTATIONS][BLOCK_LEN][BLOCK_LEN] = {
    {{
	 {'\0', 'O', 'O', '\0'}, // O-Tetrimino
	 {'\0', 'O', 'O', '\0'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', 'O', 'O', '\0'},
	 {'\0', 'O', 'O', '\0'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', 'O', 'O', '\0'},
	 {'\0', 'O', 'O', '\0'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', 'O', 'O', '\0'},
	 {'\0', 'O', 'O', '\0'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     }},
    {{
	 {'I', 'I', 'I', 'I'}, // I-Tetrimino
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', '\0', 'I', '\0'},
	 {'\0', '\0', 'I', '\0'},
	 {'\0', '\0', 'I', '\0'},
	 {'\0', '\0', 'I', '\0'},
     },
     {
	 {'I', 'I', 'I', 'I'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', '\0', 'I', '\0'},
	 {'\0', '\0', 'I', '\0'},
	 {'\0', '\0', 'I', '\0'},
	 {'\0', '\0', 'I', '\0'},
     }},
    {{
	 {'\0', '\0', 'T', '\0'}, // T-Tetrimino
	 {'\0', 'T', 'T', 'T'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', '\0', 'T', '\0'},
	 {'\0', 'T', 'T', '\0'},
	 {'\0', '\0', 'T', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', 'T', 'T', 'T'},
	 {'\0', '\0', 'T', '\0'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', 'T', '\0', '\0'},
	 {'\0', 'T', 'T', '\0'},
	 {'\0', 'T', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     }},
    {{
	 {'\0', '\0', '\0', 'L'}, // L-Tetrimino
	 {'\0', 'L', 'L', 'L'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', 'L', 'L', '\0'},
	 {'\0', '\0', 'L', '\0'},
	 {'\0', '\0', 'L', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', 'L', 'L', 'L'},
	 {'\0', 'L', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', '\0', 'L', '\0'},
	 {'\0', '\0', 'L', '\0'},
	 {'\0', '\0', 'L', 'L'},
	 {'\0', '\0', '\0', '\0'},
     }},
    {{
	 {'\0', 'J', 'J', 'J'}, // J-Tetrimino
	 {'\0', '\0', '\0', 'J'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', '\0', 'J', 'J'},
	 {'\0', '\0', 'J', '\0'},
	 {'\0', '\0', 'J', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', 'J', '\0', '\0'},
	 {'\0', 'J', 'J', 'J'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', '\0', '\0', 'J'},
	 {'\0', '\0', '\0', 'J'},
	 {'\0', '\0', 'J', 'J'},
	 {'\0', '\0', '\0', '\0'},
     }},
    {{
	 {'\0', '\0', 'S', 'S'}, // S-Tetrimino
	 {'\0', 'S', 'S', '\0'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', '\0', 'S', '\0'},
	 {'\0', '\0', 'S', 'S'},
	 {'\0', '\0', '\0', 'S'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', '\0', 'S', 'S'},
	 {'\0', 'S', 'S', '\0'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', '\0', 'S', '\0'},
	 {'\0', '\0', 'S', 'S'},
	 {'\0', '\0', '\0', 'S'},
	 {'\0', '\0', '\0', '\0'},
     }},
    {{
	 {'\0', 'Z', 'Z', '\0'}, // Z-Tetrimino
	 {'\0', '\0', 'Z', 'Z'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', '\0', '\0', 'Z'},
	 {'\0', '\0', 'Z', 'Z'},
	 {'\0', '\0', 'Z', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', 'Z', 'Z', '\0'},
	 {'\0', '\0', 'Z', 'Z'},
	 {'\0', '\0', '\0', '\0'},
	 {'\0', '\0', '\0', '\0'},
     },
     {
	 {'\0', '\0', '\0', 'Z'},
	 {'\0', '\0', 'Z', 'Z'},
	 {'\0', '\0', 'Z', '\0'},
	 {'\0', '\0', '\0', '\0'},
     }}};

static void rmtetr(void);
static void addtetr(void);
static int clear_lines(void);
static bool blocked(int offsetx, int offsety);
static Tetr *mktetr(void);

void start_game(void)
{
	int i, j;

	curtick = 0;
	score = 0;
	gameover = false;

	srand((unsigned)time(NULL));

	cur = malloc(sizeof(struct tetrimino));
	next = malloc(sizeof(struct tetrimino));
	if (cur == NULL || next == NULL)
		exit(EXIT_FAILURE);

	cur = mktetr();
	next = mktetr();

	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			board[i][j] = '-';
		}
	}
	addtetr();
}

void mvleft(void)
{
	if (gameover)
		return;

	rmtetr();
	if (!blocked(-1, 0))
		cur->posx--;
	addtetr();
}

void mvright(void)
{
	if (gameover)
		return;

	rmtetr();
	if (!blocked(1, 0))
		cur->posx++;
	addtetr();
}

void rotcw(void)
{
	if (gameover)
		return;

	rmtetr();
	cur->orientation = (cur->orientation + 1) % ORIENTATIONS;
	if (blocked(0, 0))
		cur->orientation = (cur->orientation + 3) % ORIENTATIONS;
	addtetr();
}

void rotccw(void)
{
	if (gameover)
		return;

	rmtetr();
	cur->orientation = (cur->orientation + 3) % ORIENTATIONS;
	if (blocked(0, 0))
		cur->orientation = (cur->orientation + 1) % ORIENTATIONS;
	addtetr();
}

void mvdrop(void)
{
	if (gameover)
		return;

	int newy;

	rmtetr();
	for (newy = 0; !blocked(0, newy + 1); newy++)
		;

	cur->posy += newy;
	addtetr();
	score += clear_lines();

	free(cur);
	cur = next;
	next = mktetr();
	curtick = 0;

	if (blocked(0, 0))
		gameover = true;
	else
		addtetr();
}

void tick()
{
	if (curtick < 10) {
		curtick++;
		return;
	}
	curtick = 0;

	if (gameover)
		return;

	rmtetr();

	if (!blocked(0, 1)) {
		cur->posy++;
		addtetr();
		return;
	}

	addtetr();
	score += clear_lines();

	free(cur);
	cur = next;
	next = mktetr();

	if (blocked(0, 0))
		gameover = true;
	else
		addtetr();
}

static void rmtetr(void)
{
	int i, j;

	for (i = 0; i < BLOCK_LEN; i++) {
		for (j = 0; j < BLOCK_LEN; j++) {
			if (cur->block[cur->orientation][i][j])
				board[cur->posy + i][cur->posx + j] = '-';
		}
	}
}

static void addtetr(void)
{
	int i, j;

	for (i = 0; i < BLOCK_LEN; i++) {
		for (j = 0; j < BLOCK_LEN; j++) {
			if (cur->block[cur->orientation][i][j])
				board[cur->posy + i][cur->posx + j] =
				    cur->block[cur->orientation][i][j];
		}
	}
}

static int clear_lines(void)
{
	int row, col, cnt;

	cnt = 0;
	row = ROWS - 1;
	while (row > 0) {
		for (col = 0; col < COLS && board[row][col] != '-'; col++)
			;
		if (col != COLS) {
			row--;
			continue;
		}

		for (int i = row; i - 1 > 0; i--) {
			memcpy(board[i], board[i - 1], COLS);
		}

		for (int i = 0; i < COLS; i++)
			board[0][i] = '-';
		cnt++;
	}
	return cnt;
}

static bool blocked(int offsetx, int offsety)
{
	int boardx, boardy;
	int i, j;

	boardx = cur->posx + offsetx;
	boardy = cur->posy + offsety;

	for (i = 0; i < BLOCK_LEN; i++) {
		for (j = 0; j < BLOCK_LEN; j++) {
			if (!cur->block[cur->orientation][i][j])
				continue;

			if (boardx + j < 0 || boardx + j >= COLS ||
			    boardy + i < 0 || boardy + i >= ROWS) {
				return true;
			}

			if (board[boardy + i][boardx + j] != '-')
				return true;
		}
	}
	return false;
}

Tetr *mktetr()
{
	Tetr *t;
	t = malloc(sizeof(Tetr));
	if (t == NULL)
		exit(EXIT_FAILURE);

	t->orientation = 0;
	t->block = TETRIMINO[rand() % N_TETRIMINO];
	t->posx = 3;
	t->posy = 0;

	return t;
}
