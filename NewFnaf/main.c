#define _CRT_SECURE_NO_WARNINGS

#include <curses.h>
#include <stdlib.h>
#include <time.h>

#include "draw.h"
#include "mainmenu.h"

int main() {

    srand(time(NULL));

    initscr();
    noecho();
    cbreak();
    curs_set(0);



    keypad(stdscr, TRUE);
    
    start_color();
    use_default_colors();
    initHexColors();

    showMainMenu();

}



