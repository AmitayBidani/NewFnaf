#include "draw.h"
#include <curses.h>
#include <stdbool.h>
#include "mainmenu.h"

void showMainMenu() {


    init_pair(1, COLOR_WHITE, 20);
    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, 21, COLOR_RED);


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


        attron(COLOR_PAIR(3));
        border(ACS_BLOCK, ACS_BLOCK, ACS_BLOCK, ACS_BLOCK,
            ACS_BLOCK, ACS_BLOCK, ACS_BLOCK, ACS_BLOCK);
        attroff(COLOR_PAIR(3));

        attron(A_BOLD);

        //The Title (Name Of The Game)
        int text[5][26] = {
            {1, 1, 1,0 , 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0,0 , 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1,0 , 1, 0 ,1, 1, 0, 1, 1 ,1 ,0 ,1 ,1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0,0 , 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}
        };

        int line[5][26] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        };

        drawArt(3, 3, 26, 5, 1, text);

        drawArt(9, 3, 26, 1, 1, line);


        for (int i = 0; i < 3; i++)
        {
            if (i == selected) {
                attron(COLOR_PAIR(2));
                mvprintw(12 + i, 12, options[i]);
                attroff(COLOR_PAIR(2));
            }
            else {
                attron(COLOR_PAIR(1));
                mvprintw(12 + i, 12, options[i]);
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
        case 10: // PRESSED ENTER
            running = false;
            break;
        }
        attroff(A_BOLD);
    }

    click(selected);
}



//This function is running only when you press enter!
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
