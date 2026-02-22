#include <ncurses.h>
#include <unistd.h>
#include "tetris.h"

static WINDOW *boardw;

WINDOW *create_boardw(int lines, int columns, int height, int width);
void ui_init(void);
void draw_board(Tetris state);
void drawch(int x, int y, char c);

int main(void)
{
	Tetris state = create_game();

	int ch;

	ui_init();
	boardw = create_boardw(ROWS, COLS, 1, 3);

	render_board(state, drawch);
	wrefresh(boardw);

	while ((ch = getch()) != 'c') {
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
		case 'z':
			rotccw(state);
			break;
		case ' ':
			mvdrop(state);
			break;
		}
		render_board(state, drawch);
		wrefresh(boardw);
	}
	getch();
	endwin();

	return 0;
}

void drawch(int x, int y, char c)
{
	mvwaddch(boardw, y + 1, x + 1, c);
}

void ui_init()
{
	initscr();
	cbreak();
	noecho();
	refresh();

	keypad(stdscr, TRUE);
	curs_set(0);
	timeout(100);
}

WINDOW *create_boardw(int lines, int columns, int height, int width)
{
	WINDOW *local_win;

	local_win = newwin(lines + 2, columns + 2, height, width);
	box(local_win, 0, 0);

	wrefresh(local_win);

	return local_win;
}
