#include <ncurses.h>
#include <string>
#include <vector>
#include <fstream>


void print_items(int highlighted, const std::vector<std::string> &items);

int highlight_change(int highlighted, int change, const std::vector<std::string> &items);

std::vector<std::string> move_element(std::vector<std::string> items, int index, int change);

std::vector<std::string> load_from_file(const char *filename);

void save_to_file(const std::vector<std::string> &items, const char *filename);

const char *modeToString(int mode);


enum Mode {
    NORMAL,
    INSERT,
    MOVE
};
static const char *modeStrings[] = {"NORMAL", "INSERT", "MOVE"};

int main() {
    const char *filename = "items";
    Mode mode = NORMAL;
    int highlighted = 0;
    std::vector<std::string> items = load_from_file(filename);

    initscr();
    noecho();
    curs_set(0);
    while (true) {
        int insert_mode_switch = 0;
        int yMax = ((nullptr != static_cast<const void *>((stdscr))) ? ((stdscr)->_maxy + 1) : (-1));
        const char *mode_string = modeToString(mode);

        //clear in case the screen resized or the items changed
        clear();
        mvaddstr(yMax - 1, 0, mode_string);
        print_items(highlighted, items);

        int key = wgetch(stdscr);
        if (key == 410) continue;
        if (mode == Mode::NORMAL) {
            switch (key) {
                case 'q':
                    endwin();
                    save_to_file(items, filename);
                    exit(0);
                case 'j':
                    highlighted = highlight_change(highlighted, 1, items);
                    break;
                case 'k':
                    highlighted = highlight_change(highlighted, -1, items);
                    break;
                case 'i':
                    mode = Mode::INSERT;
                    insert_mode_switch = 1;
                    break;
                case 'G':
                    highlighted = 0;
                    break;
                case 'g':
                    highlighted = static_cast<int>(items.size()) - 1;
                    break;
                case 'd':
                    items.erase(items.begin() + highlighted);
                    break;
                case 'n':
                    items.emplace_back("");
                    highlighted = static_cast<int>(items.size()) - 1;
                    mode = Mode::INSERT;
                    insert_mode_switch = 1;
                    break;
                case 'm':
                    mode = Mode::MOVE;
                    break;
                default:
                    break;
            }
        } else if (mode == Mode::MOVE) {
            switch (key) {
                case 'j':
                    items = move_element(items, highlighted, 1);
                    highlighted = highlight_change(highlighted, 1, items);
                    break;
                case 'k':
                    items = move_element(items, highlighted, -1);
                    highlighted = highlight_change(highlighted, -1, items);
                    break;
                case 'q':
                    mode = Mode::NORMAL;
                    break;
                default:
                    break;
            }
        }
        if (key == 27) { //27 is the Esc key
            mode = Mode::NORMAL;
        } else if (mode == Mode::INSERT && insert_mode_switch != 1) {
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

std::vector<std::string> move_element(std::vector<std::string> items, int index, int change) {
    std::string element = items.at(index);
    items.erase(items.begin() + index);
    int new_index = index + change;
    items.insert(items.begin() + new_index, element);

    return items;
}

std::vector<std::string> load_from_file(const char *filename) {
    std::vector<std::string> vec;
    std::ifstream file(filename);
    std::string line;
    while (getline(file, line))
        vec.emplace_back(line);
    file.close();
    return vec;
}

void save_to_file(const std::vector<std::string> &items, const char *filename) {
    std::string input;
    for (const auto &entry: items)
        input += entry + "\n";

    std::ofstream out(filename);
    out << input;
    out.close();
}

const char *modeToString(int mode) {
    return modeStrings[mode];
}