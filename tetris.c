#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tetris.h"

#define BLOCK_LEN 4
#define N_TETRIMINO 7
#define ORIENTATIONS 4

struct tetrimino {
	int orientation;
	const char (*block)[BLOCK_LEN][BLOCK_LEN];
	int posx;
	int posy;
};
typedef struct tetrimino Tetrimino;

struct tetris_state {
	int curtick;
	int score;
	int gameover;
	char board[ROWS][COLS];
	Tetrimino *cur;
	Tetrimino *next;
};

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

static void rmcurtetr(Tetris state);
static void addcurtetr(Tetris state);
static bool blocked(Tetris state, int offsetx, int offsety);
static Tetrimino *mktetr(void);
static int clear_lines(Tetris state);

Tetris create_game(void)
{
	int i, j;
	Tetris state;

	state = malloc(sizeof(struct tetris_state));
	if (state == NULL)
		exit(EXIT_FAILURE);

	state->curtick = 0;
	state->score = 0;
	state->gameover = false;

	srand((unsigned)time(NULL));

	state->cur = malloc(sizeof(struct tetrimino));
	state->next = malloc(sizeof(struct tetrimino));
	if (state->cur == NULL || state->next == NULL)
		exit(EXIT_FAILURE);

	state->cur = mktetr();
	state->next = mktetr();

	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			state->board[i][j] = '-';
		}
	}
	addcurtetr(state);

	return state;
}

void destroy_game(Tetris state)
{
	free(state->cur);
	free(state->next);
	free(state);
}

void render_board(Tetris state, void drawch(int x, int y, char c))
{
	int i, j;

	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			drawch(j, i, state->board[i][j]);
		}
	}
}

void mvleft(Tetris state)
{
	if (state->gameover)
		return;

	rmcurtetr(state);
	if (!blocked(state, -1, 0))
		state->cur->posx--;
	addcurtetr(state);
}

void mvright(Tetris state)
{
	if (state->gameover)
		return;

	rmcurtetr(state);
	if (!blocked(state, 1, 0))
		state->cur->posx++;
	addcurtetr(state);
}

void rotcw(Tetris state)
{
	if (state->gameover)
		return;

	rmcurtetr(state);
	state->cur->orientation = (state->cur->orientation + 1) % ORIENTATIONS;
	if (blocked(state, 0, 0))
		state->cur->orientation =
		    (state->cur->orientation + 3) % ORIENTATIONS;
	addcurtetr(state);
}

void rotccw(Tetris state)
{
	if (state->gameover)
		return;

	rmcurtetr(state);
	state->cur->orientation = (state->cur->orientation + 3) % ORIENTATIONS;
	if (blocked(state, 0, 0))
		state->cur->orientation =
		    (state->cur->orientation + 1) % ORIENTATIONS;
	addcurtetr(state);
}

void mvdrop(Tetris state)
{
	if (state->gameover)
		return;

	int newy;

	rmcurtetr(state);
	for (newy = 0; !blocked(state, 0, newy + 1); newy++)
		;

	state->cur->posy += newy;
	addcurtetr(state);
	state->score += clear_lines(state);

	free(state->cur);
	state->cur = state->next;
	state->next = mktetr();
	state->curtick = 0;

	if (blocked(state, 0, 0))
		state->gameover = true;
	else
		addcurtetr(state);
}

void tick(Tetris state)
{
	if (state->curtick < 10) {
		state->curtick++;
		return;
	}
	state->curtick = 0;

	if (state->gameover)
		return;

	rmcurtetr(state);

	if (!blocked(state, 0, 1)) {
		state->cur->posy++;
		addcurtetr(state);
		return;
	}

	addcurtetr(state);
	state->score += clear_lines(state);

	free(state->cur);
	state->cur = state->next;
	state->next = mktetr();

	if (blocked(state, 0, 0))
		state->gameover = true;
	else
		addcurtetr(state);
}

static void rmcurtetr(Tetris state)
{
	int i, j;

	for (i = 0; i < BLOCK_LEN; i++) {
		for (j = 0; j < BLOCK_LEN; j++) {
			if (state->cur->block[state->cur->orientation][i][j])
				state->board[state->cur->posy + i]
					    [state->cur->posx + j] = '-';
		}
	}
}

static void addcurtetr(Tetris state)
{
	int i, j;

	for (i = 0; i < BLOCK_LEN; i++) {
		for (j = 0; j < BLOCK_LEN; j++) {
			if (state->cur->block[state->cur->orientation][i][j])
				state->board[state->cur->posy + i]
					    [state->cur->posx + j] =
				    state->cur
					->block[state->cur->orientation][i][j];
		}
	}
}

static int clear_lines(Tetris state)
{
	int row, col, cnt;

	cnt = 0;
	row = ROWS - 1;
	while (row > 0) {
		for (col = 0; col < COLS && state->board[row][col] != '-';
		     col++)
			;
		if (col != COLS) {
			row--;
			continue;
		}

		for (int i = row; i - 1 > 0; i--) {
			memcpy(state->board[i], state->board[i - 1], COLS);
		}

		for (int i = 0; i < COLS; i++)
			state->board[0][i] = '-';
		cnt++;
	}
	return cnt;
}

static bool blocked(Tetris state, int offsetx, int offsety)
{
	int boardx, boardy;
	int i, j;

	boardx = state->cur->posx + offsetx;
	boardy = state->cur->posy + offsety;

	for (i = 0; i < BLOCK_LEN; i++) {
		for (j = 0; j < BLOCK_LEN; j++) {
			if (!state->cur->block[state->cur->orientation][i][j])
				continue;

			if (boardx + j < 0 || boardx + j >= COLS ||
			    boardy + i < 0 || boardy + i >= ROWS) {
				return true;
			}

			if (state->board[boardy + i][boardx + j] != '-')
				return true;
		}
	}
	return false;
}

Tetrimino *mktetr()
{
	Tetrimino *t;
	t = malloc(sizeof(struct tetrimino));
	if (t == NULL)
		exit(EXIT_FAILURE);

	t->orientation = 0;
	t->block = TETRIMINO[rand() % N_TETRIMINO];
	t->posx = 3;
	t->posy = 0;

	return t;
}
