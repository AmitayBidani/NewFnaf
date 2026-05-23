#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "draw.h"
#include "images.h"

#include "mainmenu.h"
#include "game.h"

#include "saver.h"


void showMainMenu() {

    Data data;
    loadData(&data);

    init_pair(1, getColor(255, 255, 255), getColor(215, 0, 0));
    init_pair(2, getColor(255,255,255), getColor(175, 0, 0));
    init_pair(3, getColor(255, 255, 255), getColor(18, 18, 18));

    int selected = 0;
    int key = 0;
    int optionSize = 5;

    char** options;
    if (data.day == 0 && data.hour == 0 && data.battery == 100 && data.wins == 0) {

        optionSize = 4;
        options = malloc(optionSize * sizeof(char[50]));

        if (options == NULL)
            return;

        options[0] = "- New Game";
        options[1] = "- How To Play";
        options[2] = "- Credits";
        options[3] = "- Quit";
    }
    else {
        options = malloc(optionSize * sizeof(char[50]));

        if (options == NULL)
            return;

        options[0] = "- Continue";
        options[1] = "- New Game";
        options[2] = "- How To Play";
        options[3] = "- Credits";
        options[4] = "- Quit";

    }

    

    bool running = true;
    while (running) {

        if (key != -1) {
            erase();
            drawImage(0, 0, MENU_WIDTH, MENU_HEIGHT, menu_pixels, 1);


            for (int i = 0; i < optionSize; i++)
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
                selected = (selected - 1 + optionSize) % optionSize;
                break;
            case KEY_DOWN:
                selected = (selected + 1) % optionSize;
                break;
            case 10: // PRESSED ENTER
                running = false;
                break;
            }
        }
    }

    
    click(options[selected], data);

    free(options);
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
void click(char* selected, Data data) {

    if (strcmp(selected, "- Continue") == 0) {
        loadScreen();
        napms(700);
        showGame(data);
    }
    else if (strcmp(selected, "- New Game") == 0) {
        loadScreen();
        data = (Data){ 0,0,100,0 };
        napms(700);
        showGame(data);
    }
    else if (strcmp(selected, "- How To Play") == 0) {
        while (1) {
            clear();
            mvprintw(5, 5, "in How To Play!!");

            refresh();
        }
    }
    else if (strcmp(selected, "- Credits") == 0) {
        while (1) {
            clear();
            mvprintw(5, 5, "in Credits!!");

            refresh();
        }
    }
    else if (strcmp(selected, "- Quit") == 0) {
        return;
    }
}
