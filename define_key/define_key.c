#include <stdlib.h>
#include <ncurses.h>

WINDOW *WIN;

int WIDTH =  0;
int HEIGHT = 0;

int main() {
	initscr();

    /* SETUP */
    cbreak();
    noecho();
    curs_set(false);
    nodelay(stdscr, false);
    keypad(stdscr, true);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_MAGENTA);
    getmaxyx(stdscr, HEIGHT, WIDTH);
    
    refresh();

    /* WINDOW */
    WIN = newwin(HEIGHT, WIDTH, 0, 0);
    box(WIN, 0, 0);
    wattron(WIN, COLOR_PAIR(1));
    mvwhline(WIN, 0, 1, ' ', WIDTH - 2);
    mvwprintw(WIN, 0, 2, "DEFINE_KEY");
    mvwprintw(WIN, 0, WIDTH - 11, "Exit: ESC");
    wattroff(WIN, COLOR_PAIR(1));
    wrefresh(WIN);
            
    /* INPUT */
	int ch;
	int x = 2, y = 2;
	while ((ch = getch()) != 27) {
        if (x <= (WIDTH - 25)) {
            mvprintw(y, x, "Keycode: %i, name: %c", ch, ch);
            y++;
            
            if (y >= (HEIGHT - 2)) {
                y = 2;
                x += 30;
            }
        }
	}

	endwin();
    
    return 0;
};
