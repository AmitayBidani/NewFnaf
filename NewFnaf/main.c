#include <curses.h>

int main() {
    initscr();
    printw("Hello, PDCurses!");
    refresh();
    getch();
    endwin();
    return 0;
}

