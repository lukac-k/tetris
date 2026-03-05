#include <ncurses.h>
#include <unistd.h>
#include "tetris.h"

static WINDOW *boardw;
static WINDOW *nextw;
static WINDOW *scorew;

WINDOW *create_boardw(int lines, int columns, int height, int width);
void drawch(void *ctx, int x, int y, char c);
int color_pair(char c);

int main(void)
{
	Tetris state = create_game();
	int ch;

	/* Initial ui setup */
	initscr();

	start_color();
	init_pair(1, COLOR_GREEN, COLOR_YELLOW);
	init_pair(2, COLOR_GREEN, COLOR_CYAN);
	init_pair(3, COLOR_GREEN, COLOR_MAGENTA);
	init_pair(4, COLOR_GREEN, COLOR_WHITE);
	init_pair(5, COLOR_GREEN, COLOR_BLUE);
	init_pair(6, COLOR_GREEN, COLOR_GREEN);
	init_pair(7, COLOR_GREEN, COLOR_RED);

	cbreak();
	noecho();
	refresh();
	keypad(stdscr, TRUE);
	curs_set(0);
	timeout(100);

	attr_on(A_BOLD, 0);
	mvprintw(1, 6, "TETRIS");
	attr_off(A_BOLD, 0);

	/* Create board window */
	boardw = newwin(ROWS + 2, COLS * 2 + 2, 2, 3);
	render_board(state, boardw, drawch);
	box(boardw, 0, 0);
	wrefresh(boardw);

	/* Create next tetrinimo window */
	nextw = newwin(BLOCK_LEN + 2, BLOCK_LEN * 2 + 2, 2, 30);
	render_next(state, nextw, drawch);
	box(nextw, 0, 0);
	mvwprintw(nextw, 0, 2, "Next");
	wrefresh(nextw);

	/* Create score window */
	scorew = newwin(3, BLOCK_LEN * 2 + 2, 8, 30);
	box(scorew, 0, 0);
	mvwprintw(scorew, 0, 2, "Score");
	mvwprintw(scorew, 1, 5, "%d", get_score(state));
	wrefresh(scorew);

	while ((ch = getch()) != 'q') {
		tick(state);

		switch (ch) {
		case KEY_LEFT:
			mvleft(state);
			break;
		case KEY_RIGHT:
			mvright(state);
			break;
		case 'x':
		case KEY_UP:
			rotcw(state);
			break;
		case KEY_DOWN:
			tick(state);
			break;
		case 'z':
			rotccw(state);
			break;
		case ' ':
			mvdrop(state);
			break;
		}
		render_board(state, boardw, drawch);
		wrefresh(boardw);
		render_next(state, nextw, drawch);
		wrefresh(nextw);
		mvwprintw(scorew, 1, 5, "%d", get_score(state));
		wrefresh(scorew);
	}
	getch();
	delwin(boardw);
	delwin(nextw);
	destroy_game(state);
	endwin();

	return 0;
}

void drawch(void *ctx, int x, int y, char c)
{
	WINDOW *win = ctx;
	mvwaddch(win, y + 1, x * 2 + 1, ' ' | COLOR_PAIR(color_pair(c)));
	mvwaddch(win, y + 1, x * 2 + 2, ' ' | COLOR_PAIR(color_pair(c)));
}

int color_pair(char c)
{
	switch (c) {
	case 'O':
		return 1;
	case 'I':
		return 2;
	case 'T':
		return 3;
	case 'J':
		return 4;
	case 'L':
		return 5;
	case 'S':
		return 6;
	case 'Z':
		return 7;
	default:
		return 0;
	}
}
