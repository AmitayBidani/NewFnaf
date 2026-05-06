#define _CRT_SECURE_NO_WARNINGS
#include <curses.h>

int main() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);

    start_color();

    init_color(20, 235, 522, 902);
    init_pair(1, COLOR_WHITE, 20);
    init_pair(2, COLOR_WHITE, 20);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);


    int selected = 0;
    int key;
    char* options[] = {
        "- Start",
        "- How To Play",
        "- Quit"
    };

    while (1) {
        clear();
        wbkgd(stdscr, COLOR_PAIR(1));

        attron(A_BOLD);
        mvprintw(7, 10, "FNAF 2");
        attroff(A_BOLD);

        

        for (int i = 0; i < 3; i++)
        {
            if (i == selected) {
                attron(COLOR_PAIR(3));
                mvprintw(10 + i, 10, options[i]);
                attroff(COLOR_PAIR(3));
            }
            else {
                attron(COLOR_PAIR(2));
                mvprintw(10 + i, 10, options[i]);
                attroff(COLOR_PAIR(2));
            }
        }

        mvprintw(15, 10, "(%d)", selected);
        
        refresh();
        
        key = getch();
        switch (key) {
            case KEY_UP:
                selected = (selected - 1 + 3) % 3;
                break;
            case KEY_DOWN:
                selected = (selected + 1) % 3;
                break;
            case ALT_ENTER:
                break;

        }

    }

}