#include <ncurses.h>
#include <unistd.h>

#include "game.h"

static WINDOW *boardw;

WINDOW *create_boardw(int lines, int columns, int height, int width);
void ui_init(void);
void draw_board(int n, int m, char board[n][m]);

int main(void)
{
	int ch;

	ui_init();
	boardw = create_boardw(ROWS, COLS, 1, 3);

	start_game();
	draw_board(ROWS, COLS, board);

	while ((ch = getch()) != 'c') {
		tick();

		switch (ch) {
		case KEY_LEFT:
			mvleft();
			break;
		case KEY_RIGHT:
			mvright();
			break;
		case 'x':
		case KEY_UP:
			rotcw();
			break;
		case 'z':
			rotccw();
			break;
		case ' ':
			mvdrop();
			break;
		}
		draw_board(ROWS, COLS, board);
	}
	getch();
	endwin();

	return 0;
}

void draw_board(int n, int m, char b[n][m])
{
	int i, j;

	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			mvwaddch(boardw, i + 1, j + 1, b[i][j]);
		}
	}
	wrefresh(boardw);
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
