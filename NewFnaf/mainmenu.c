#include <curses.h>
#include <stdbool.h>

#include "draw.h"
#include "images.h"

#include "mainmenu.h"
#include "game.h"




void showMainMenu() {


    init_pair(1, getColor(255, 255, 255), getColor(215, 0, 0));
    init_pair(2, getColor(255,255,255), getColor(175, 0, 0));
    init_pair(3, getColor(255, 255, 255), getColor(18, 18, 18));

    int selected = 0;
    int key;
    char* options[] = {
        "- Start",
        "- How To Play",
        "- Credits",
        "- Quit"
    };

    bool running = true;
    while (running) {
        clear();

        drawImage(0, 0, MENU_WIDTH, MENU_HEIGHT, menu_pixels);


        for (int i = 0; i < 4; i++)
        {
            if (i == selected) {
                attron(COLOR_PAIR(2));
                mvprintw(15 + i, 26, options[i]);
                attroff(COLOR_PAIR(2));
            }
            else {
                attron(COLOR_PAIR(1));
                mvprintw(15 + i, 26, options[i]);
                attroff(COLOR_PAIR(1));
            }
        }
        attron(COLOR_PAIR(3));
        mvprintw(29, 2, "Arrow Up - Up   Arrow Down - Down   Enter - Select");
        attroff(COLOR_PAIR(3));

        refresh();

        key = getch();



        switch (key) {
        case KEY_UP:
            selected = (selected - 1 + 4) % 4;
            break;
        case KEY_DOWN:
            selected = (selected + 1) % 4;
            break;
        case 10: // PRESSED ENTER
            running = false;
            break;
        }

    }

    click(selected);
}

void loadScreen() {
    clear();
    wbkgd(stdscr, COLOR_PAIR(3));

    attron(COLOR_PAIR(3));
    mvprintw(15, 56, "Loading..");
    attroff(COLOR_PAIR(3));

    refresh();
}



//This function is running only when you press enter!
void click(int selected) {

    
    switch (selected) {
        case 0:
            loadScreen();
            napms(700);
            showGame();
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
