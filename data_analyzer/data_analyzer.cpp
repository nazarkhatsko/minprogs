#include <string>
#include <vector>
#include <fstream>
#include <ncurses.h>

WINDOW *WIN_MAIN;
WINDOW *WIN_ANALYZER;

int WIDTH = 0;
int HEIGHT = 0;

void win_analyzer(const std::string path);

int main(int argc, const char *argv[]) {
    initscr();

    /* SETUP */
    cbreak();
    noecho();
    curs_set(false);
    nodelay(stdscr, false);
    keypad(stdscr, true);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    getmaxyx(stdscr, HEIGHT, WIDTH);
    
    refresh();
     
    /* WINDOW */
    WIN_MAIN = newwin(HEIGHT, WIDTH, 0, 0);
    box(WIN_MAIN, 0, 0);
    wattron(WIN_MAIN, COLOR_PAIR(1));
    mvwhline(WIN_MAIN, 1, 2, ' ', WIDTH - 4);
    mvwprintw(WIN_MAIN, 1, 3, "DATA_ANALYZER");
    mvwprintw(WIN_MAIN, 1, WIDTH - 12, "Exit: ESC");
    wattroff(WIN_MAIN, COLOR_PAIR(1));
    mvwprintw(WIN_MAIN, HEIGHT - 2, 2, "Path: ");
    wrefresh(WIN_MAIN);
    
    WIN_ANALYZER = newwin(HEIGHT - 10, WIDTH - 4, 5, 2);
    wrefresh(WIN_ANALYZER);
        
    if (argc > 1) {
        win_analyzer(argv[1]);
    }
        
    /* INPUT */
    char ch;
    std::string str = "";
    while ((ch = wgetch(WIN_MAIN)) != 27) {
        if (ch == 10) {
            win_analyzer(str);
            str = "";
            mvwhline(WIN_MAIN, HEIGHT - 2, 8, ' ', WIDTH - 10);
        } else if (ch == 127) {
            str.pop_back();
            mvwhline(WIN_MAIN, HEIGHT - 2, 8, ' ', WIDTH - 10);
        }
        
        ch != 10 && ch != 127 && str.size() < WIDTH - 10 ? str += ch : str; // \n == 10 delete == 127 ASCII
        mvwprintw(WIN_MAIN, HEIGHT - 2, 2, "Path: %s", str.c_str());
    }
    
    endwin();

    return 0;
}

void win_analyzer(const std::string path) {
    wclear(WIN_ANALYZER);

    std::ifstream file;
    file.open(path);

    if (file.is_open()) {
        std::vector<std::string> words;
        std::vector<int> counts;
        
        while (!file.eof()) {
            std::string str = "";
            file >> str;

            std::vector<std::string>::iterator it_words = find(words.begin(), words.end(), str);
            if (it_words != words.end()) {
                int index = std::distance(words.begin(), it_words);
                counts[index]++;
            } else {
                if (str.size() >= 40) {
                    str.erase(str.begin() + 35, str.end());
                    str += "...";
                }
                    
                words.push_back(str);
                counts.push_back(1);
            }
        }
        
        for (int i = 0; i < words.size(); i++) {
            std::vector<int>::iterator it_counts = std::max_element(counts.begin() + i, counts.end());
            const int index = std::distance(counts.begin(), it_counts);
            std::swap(words[i], words[index]);
            std::swap(counts[i], counts[index]);
        }
                
        wattron(WIN_ANALYZER, COLOR_PAIR(2));
        mvwprintw(WIN_ANALYZER, 0, 0, " %s ", path.c_str());
        wattroff(WIN_ANALYZER, COLOR_PAIR(2));
        
        const int count = (words.size() + counts.size()) / (HEIGHT - 15) + 0.999999;
        for (int i = 0, n = 0; i < count; i++, n += 40) {
            mvwhline(WIN_ANALYZER, 0, path.size() + n + 2, '-', 40);
            mvwvline(WIN_ANALYZER, 1, path.size() + n, '|', 3);
            mvwvline(WIN_ANALYZER, 1, path.size() + n + 40, '|', 3);
            
            i != (count - 1) ? mvwhline(WIN_ANALYZER, 0, path.size() + n + 42, '-', 40) : i;
        }
        
        int x = path.size(), y = 5;
        for (int i = 0; i < words.size(); i++, y++) {
            mvwprintw(WIN_ANALYZER, y, x, "%s", words[i].c_str());
            mvwprintw(WIN_ANALYZER, y, x + 40, "%i", counts[i]);
            
            if (y == (HEIGHT - 10)) {
                y = 4;
                x += 80;
            }
        }
    }
    
    file.close();
    
    wrefresh(WIN_ANALYZER);
}
