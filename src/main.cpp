#include <ncurses.h>
#include <string>
#include <vector>


void print_items(int highlighted, const std::vector<std::string> &items);

const char *modeToString(int mode);

enum Mode {
    NORMAL,
    INSERT
};
static const char *modeStrings[] = {"NORMAL", "INSERT"};


int main() {
    Mode mode = NORMAL;
    int highlighted = 0;
    std::vector<std::string> items;
    items.emplace_back("Item1");
    items.emplace_back("Item2");
    items.emplace_back("Item3");

    initscr();
    noecho();
    curs_set(0);
    while (true) {
        int yMax = getmaxy(stdscr);
        const char *mode_string = modeToString(mode);
        mvaddstr(yMax - 1, 0, mode_string);

        print_items(highlighted, items);
        int key = getch();
        if (mode == Mode::NORMAL && key == 'q') {
            break;
        } else if (mode == Mode::NORMAL && key == 'i') {
            mode = Mode::INSERT;
        } else if (mode == Mode::INSERT && key == 27) { //27 is the Esc key
            mode = Mode::NORMAL;
        } else if (key == 410) {
            clear();
        }
    }
    endwin();

    return 0;
}

void print_items(int highlighted, const std::vector<std::string> &items) {
    wmove(stdscr, 0, 0);
    int index = 0;
    for (auto entry: items) {
        entry += "\n";
        if (index == highlighted) {
            wattron(stdscr, A_STANDOUT);
            printw(entry.c_str());
            wattroff(stdscr, A_STANDOUT);
        } else {
            printw(entry.c_str());
        }

        index++;
    }
}

const char *modeToString(int mode) {
    return modeStrings[mode];
}