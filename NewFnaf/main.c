#define _CRT_SECURE_NO_WARNINGS

#include <curses.h>

#include "draw.h"
#include "mainmenu.h"


int main() {
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



