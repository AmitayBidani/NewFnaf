#include "draw.h"
#include <curses.h>
#include <stdbool.h>
#include "mainmenu.h"

void showMainMenu() {


    init_pair(1, COLOR_WHITE, 20);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);


    int selected = 0;
    int key;
    char* options[] = {
        "- Start",
        "- How To Play",
        "- Quit"
    };

    bool running = true;
    while (running) {
        clear();
        wbkgd(stdscr, COLOR_PAIR(1));

        attron(A_BOLD);

        int text[5][16] = {
            {1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
            {1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1},
            {1, 1, 0, 1, 0 ,1, 1, 0, 1, 1 ,1 ,0 ,1 ,1, 0, 1},
            {1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1}
        };

        drawArt(3, 3, 16, 5, 1, text);


        for (int i = 0; i < 3; i++)
        {
            if (i == selected) {
                attron(COLOR_PAIR(2));
                mvprintw(10 + i, 10, options[i]);
                attroff(COLOR_PAIR(2));
            }
            else {
                attron(COLOR_PAIR(1));
                mvprintw(10 + i, 10, options[i]);
                attroff(COLOR_PAIR(1));
            }
        }

        refresh();

        key = getch();
        switch (key) {
        case KEY_UP:
            selected = (selected - 1 + 3) % 3;
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 3;
            break;
        case 10: // IF PRESSED ENTER
            running = false;
            break;
        }
        attroff(A_BOLD);
    }

    click(selected);
}


void click(int selected) {
    switch (selected) {
        case 0:
            while (1) {
                clear();
                mvprintw(5, 5, "in game!!");

                refresh();
            }
            break;
        case 1:
            while (1) {
                clear();
                mvprintw(5, 5, "in about!!");

                refresh();
            }
            break;
        case 2:
            return;
            break;

    }
}
