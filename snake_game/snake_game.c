#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

WINDOW *WIN_MAIN;
WINDOW *WIN_STATISTICS;

int WIDTH = 0;
int HEIGHT = 0;

int HP = 0;
int LEVEL = 0;
double TIMER = 0;

int STATUS = 1;

struct Cell {
    int x;
    int y;
    int color;
};

int size;
struct Cell *snake;
struct Cell hp;

void win_statistics();
void add_ui(struct Cell cell);
void remove_ui(struct Cell cell);

void new_snake();
void new_hp();

int main() {
    initscr();

    /* SETUP */
    cbreak();
    noecho();
    curs_set(false);
    nodelay(stdscr, false);
    keypad(stdscr, true);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_WHITE);
    init_pair(3, COLOR_CYAN, COLOR_CYAN);
    getmaxyx(stdscr, HEIGHT, WIDTH);
    
    refresh();
    
    /* START */
    attron(COLOR_PAIR(1));
    mvprintw(((HEIGHT / 2) - 4), ((WIDTH / 2) - 48), "                                                                                                        ");
    mvprintw(((HEIGHT / 2) - 3), ((WIDTH / 2) - 49), "                                                                                                        ");
    mvprintw(((HEIGHT / 2) - 2), ((WIDTH / 2) - 50), "     |-------  ||    |  |------|  |    |  |------        |-------  |------|  ||     ||  |------         ");
    mvprintw(((HEIGHT / 2) - 1), ((WIDTH / 2) - 51), "      |         | |   |  |      |  |  |    |              |         |      |  |  | |  |  |              ");
    mvprintw(((HEIGHT / 2) + 0), ((WIDTH / 2) - 52), "       |------|  |  |  |  |------|  ||      |------        |   ---|  |------|  |   |   |  |------       ");
    mvprintw(((HEIGHT / 2) + 1), ((WIDTH / 2) - 53), "               |  |   | |  |      |  |  |    |              |      |  |      |  |       |  |            ");
    mvprintw(((HEIGHT / 2) + 2), ((WIDTH / 2) - 54), "         |------|  |    ||  |      |  |    |  |------        |------|  |      |  |       |  |------     ");
    mvprintw(((HEIGHT / 2) + 3), ((WIDTH / 2) - 55), "                                                                                                        ");
    mvprintw(((HEIGHT / 2) + 4), ((WIDTH / 2) - 56), "                                                                                                        ");
    attroff(COLOR_PAIR(1));
    mvprintw(((HEIGHT / 2) + 10), ((WIDTH / 2) - 19), "press any key to start the game....");
    getch();
    clear();
    
    /* WINDOW */
    WIN_MAIN = newwin(HEIGHT, WIDTH, 0, 0);
    nodelay(WIN_MAIN, true);
    box(WIN_MAIN, 0, 0);
    wrefresh(WIN_MAIN);
    
    WIN_STATISTICS = newwin(20, 25, 0, 0);
    win_statistics();
    wrefresh(WIN_STATISTICS);

    new_snake();
    new_hp();
    
    /* LOGIC */
    int movement = 119;
    while (STATUS == 1 || STATUS == 2) {
        int key = wgetch(WIN_MAIN);
        if (key == 119 || key == 115 || key == 100 || key == 97) {
            movement = key;
        } else if (key == 32) {
            STATUS = (STATUS == 1) ? 2 : 1;
        } else if (key == 27) {
            STATUS = 3;
        }

        if (STATUS == 1) {
            remove_ui(snake[size - 1]);
            
            for (int i = (size - 1); i > 0; i--) {
                snake[i] = snake[i - 1];
            }
            
            movement == 119 ? snake[0].y -= 1 : snake[0].y;     // TOP
            movement == 115 ? snake[0].y += 1 : snake[0].y;     // BOTTOM
            movement == 100 ? snake[0].x += 2 : snake[0].x;     // RIGHT
            movement == 97 ? snake[0].x -= 2 : snake[0].x;      // LEFT
            
            add_ui(snake[0]);
            
            if ((snake[0].y == hp.y) && (snake[0].x == hp.x || snake[0].x == hp.x + 1 || snake[0].x == hp.x - 1)) {
                HP++;
                new_hp(&hp);
                add_ui(snake[0]);
                
                size++;
                snake = (struct Cell *) realloc(snake, size);
                snake[size - 1] = snake[size - 2];
            }
            
            if ((snake[0].x < 1 || snake[0].y < 1) || (snake[0].x > (WIDTH - 2) || snake[0].y > (HEIGHT - 2)) || (snake[0].x < 25 && snake[0].y < 20)) {
                STATUS = 3;
            }
            
            LEVEL = HP / 10;
            TIMER += 0.1;
            win_statistics();
        }
        
        wrefresh(WIN_MAIN);
        napms(100);
    }
        
    /* GAME OVER */
    clear();
    attron(COLOR_PAIR(1));
    mvprintw(((HEIGHT / 2) - 4), ((WIDTH / 2) - 43), "                                                                                                ");
    mvprintw(((HEIGHT / 2) - 3), ((WIDTH / 2) - 44), "                                                                                                ");
    mvprintw(((HEIGHT / 2) - 2), ((WIDTH / 2) - 45), "     |-------  |------|  ||     ||  |------        |------|  |       |  |------  |------|       ");
    mvprintw(((HEIGHT / 2) - 1), ((WIDTH / 2) - 46), "      |         |      |  |  | |  |  |              |      |   |     |   |        |      |      ");
    mvprintw(((HEIGHT / 2) + 0), ((WIDTH / 2) - 47), "       |   ---|  |------|  |   |   |  |------        |      |    |   |    |------  |------|     ");
    mvprintw(((HEIGHT / 2) + 1), ((WIDTH / 2) - 48), "        |      |  |      |  |       |  |              |      |     | |     |        | |         ");
    mvprintw(((HEIGHT / 2) + 2), ((WIDTH / 2) - 49), "         |------|  |      |  |       |  |------        |------|      |      |------  |    |     ");
    mvprintw(((HEIGHT / 2) + 3), ((WIDTH / 2) - 50), "                                                                                                ");
    mvprintw(((HEIGHT / 2) + 4), ((WIDTH / 2) - 51), "                                                                                                ");
    attroff(COLOR_PAIR(1));
    mvprintw(((HEIGHT / 2) + 10), ((WIDTH / 2) - 19), "press any key to close the game....");
    getch();
    
    endwin();
    
    return 0;
};

void win_statistics() {
    wclear(WIN_STATISTICS);
    box(WIN_STATISTICS, 0, 0);
    wattron(WIN_STATISTICS, COLOR_PAIR(1));
    mvwprintw(WIN_STATISTICS, 1, 2, "      SNAKE_GAME     ");
    wattroff(WIN_STATISTICS, COLOR_PAIR(1));
    mvwprintw(WIN_STATISTICS, 3, 2, "HP: %i", HP);
    mvwprintw(WIN_STATISTICS, 5, 2, "Level: %i", LEVEL);
    mvwprintw(WIN_STATISTICS, 7, 2, "Time: %is", (int)TIMER);
    mvwprintw(WIN_STATISTICS, 14, 2, "Pause: spase");
    mvwprintw(WIN_STATISTICS, 16, 2, "End Game: ESC");
    mvwprintw(WIN_STATISTICS, 18, 2, "Control: W, S, D, A");
    wrefresh(WIN_STATISTICS);
};

void add_ui(struct Cell cell) {
    wattron(WIN_MAIN, COLOR_PAIR(cell.color));
    mvwprintw(WIN_MAIN, cell.y, cell.x, "  ");
    wattroff(WIN_MAIN, COLOR_PAIR(cell.color));
};

void remove_ui(struct Cell cell) {
    mvwprintw(WIN_MAIN, cell.y, cell.x, "  ");
};

void new_snake() {
    size = 10;
    snake = (struct Cell *) calloc(sizeof(struct Cell), size);

    for (int i = 0; i < size; i++) {
        mvwprintw(WIN_MAIN, snake[i].y, snake[i].x, "  ");
        
        snake[i].x = (WIDTH - 1) / 2;
        snake[i].y = (HEIGHT - 5) / 2 + i;
        snake[i].color = 2;
        
        wattron(WIN_MAIN, COLOR_PAIR(snake[i].color));
        mvwprintw(WIN_MAIN, snake[i].y, snake[i].x, "  ");
        wattroff(WIN_MAIN, COLOR_PAIR(snake[i].color));
    }
};

void new_hp() {
    mvwprintw(WIN_MAIN, hp.y, hp.x, "  ");
        
    while (true) {
        srand(time(0));
        const int rand_x = rand() % ((WIDTH - 1) - 1) + 1;
        const int rand_y = rand() % ((HEIGHT - 1) - 1) + 1;

        if (!(rand_x < 25 && rand_y < 20)) {
            hp.x = rand_x;
            hp.y = rand_y;
            break;
        }
    }
    hp.color = 3;
    
    wattron(WIN_MAIN, COLOR_PAIR(hp.color));
    mvwprintw(WIN_MAIN, hp.y, hp.x, "  ");
    wattroff(WIN_MAIN, COLOR_PAIR(hp.color));
};
