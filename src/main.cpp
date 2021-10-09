#include <ncurses.h>
#include <string>
#include <vector>
#include <iostream>


void print_items(int highlighted, const std::vector<std::string> &items);

const char *modeToString(int mode);

int highlight_change(int highlighted, int change, const std::vector<std::string> &items);

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
        if (key == 410) { //410 is the code for a screen resize
            clear();
            continue;
        }
        if (mode == Mode::NORMAL) {
            switch (key) {
                case 'q':
                    endwin();
                    exit(0);
                case 'j':
                    highlighted = highlight_change(highlighted, 1, items);
                    break;
                case 'k':
                    highlighted = highlight_change(highlighted, -1, items);
                    break;
                case 'i':
                    mode = Mode::INSERT;
                    break;
                default:
                    break;
            }
        } else if (key == 27) { //27 is the Esc key
            mode = Mode::NORMAL;
        } else {
            if (key == 127) { //Backspace
                std::string at = items.at(highlighted);
                if (!at.empty()) {
                    at.pop_back();
                    items[highlighted] = at;
                }
            } else {
                std::string at = items.at(highlighted);
                at.push_back(char(key));
                items[highlighted] = at;
            }
        }
    }
}

void print_items(int highlighted, const std::vector<std::string> &items) {
    //move to the beginning of the screen
    wmove(stdscr, 0, 0);
    int size = static_cast<int>(items.size());
    for (int i = 0; i < size; i++) {
        std::string entry = items.at(i) + "\n";
        if (i == highlighted) {
            wattron(stdscr, A_STANDOUT);
            printw(entry.c_str());
            wattroff(stdscr, A_STANDOUT);
        } else {
            printw(entry.c_str());
        }
    }
}

int highlight_change(int highlighted, int change, const std::vector<std::string> &items) {
    int item_size = static_cast<int>(items.size());
    int res = highlighted + change;
    if (res > item_size - 1 || res < 0) {
        return highlighted;
    }
    return highlighted + change;
}

const char *modeToString(int mode) {
    return modeStrings[mode];
}
