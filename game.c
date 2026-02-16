#include <string.h>
#include <ncurses.h>
#include <stdlib.h>
#include "game.h"

struct tetronimo {
	char tetr;
	int posx;
	int posy;
};
typedef struct tetronimo Tetr;

char board[ROWS][COLS];
static Tetr *cur;
static Tetr *next;
// Testchar
static char letter = 'A';

static int curtick;
static int score;

static void clear_board(void);
static void rmtetr(void);
static void addtetr(void);
static int rmlines(void);
static bool blocked(int offsetx, int offsety);

void start_game(void)
{
	curtick = 0;
	score = 0;
	cur = malloc(sizeof(struct tetronimo));
	next = malloc(sizeof(struct tetronimo));
	if (cur == NULL || next == NULL)
		exit(EXIT_FAILURE);

	cur->tetr = letter++;
	cur->posx = 5;
	cur->posy = 0;
	next->tetr = letter++;
	next->posx = 5;
	next->posy = 0;

	clear_board();
	addtetr();
}

void mvleft(void)
{
	if (!blocked(-1, 0)) {
		rmtetr();
		cur->posx--;
		addtetr();
	}
}

void mvright(void)
{
	if (!blocked(1, 0)) {
		rmtetr();
		cur->posx++;
		addtetr();
	}
}

void mvdrop(void)
{
	// find y-value to land upon
	int offsety;

	for (offsety = 0; !blocked(0, offsety + 1); offsety++)
		;

	if (blocked(0, offsety))
		return;
	rmtetr();
	cur->posy += offsety;
	addtetr();

	*cur = *next;
	*next = (struct tetronimo){.tetr = letter++, .posx = 5, .posy = 0};
	score += rmlines();
	curtick = 0;
	if (!blocked(0, 0))
		addtetr();
}

static void clear_board(void)
{
	int i, j;

	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLS; j++) {
			board[i][j] = '-';
		}
	}
}

void tick()
{
	if (curtick < 10) {
		curtick++;
		return;
	}
	curtick = 0;
	if (!blocked(0, 1)) {
		rmtetr();
		cur->posy++;
		addtetr();
	} else {
		*cur = *next;
		*next =
		    (struct tetronimo){.tetr = letter++, .posx = 5, .posy = 0};
		score += rmlines();
		if (!blocked(0, 0))
			addtetr();
	}
}

void rmtetr(void)
{
	// delete current tetr from board
	board[cur->posy][cur->posx] = '-';
	return;
}

void addtetr(void)
{
	// add current tetr from board
	board[cur->posy][cur->posx] = cur->tetr;
	return;
}

// check if any complete lines to delete
int rmlines(void)
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
	int newx, newy;

	newx = cur->posx + offsetx;
	newy = cur->posy + offsety;
	if (newx < 0 || newx >= COLS || newy < 0 || newy >= ROWS) {
		return true;
	}

	return board[newy][newx] != '-';
}

// (cur->posx + 1 < COLS && board[cur->posy][cur->posx + 1] == '-')
