#include <curses.h>
#include <stdbool.h>

#include "draw.h"
#include "images.h"

void showGame() {
    nodelay(stdscr, TRUE);
    init_pair(5, getColor(255, 255, 255), getColor(29, 29, 29));
    init_pair(6, getColor(255, 215, 0), getColor(29, 29, 29));

    while (1) {
        clear();


        // START

        drawImage(0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, background_pixels);
        attron(COLOR_PAIR(5));
        mvprintw(25, 5, "Battery:");


        mvprintw(27, 5, "Controls: ");
        mvprintw(27, 5, "C - Open camera   L - Use Flashlight   M - Toggle Mask ");
        mvprintw(28, 5, "Q - Quit");

        attroff(COLOR_PAIR(5));


        attron(COLOR_PAIR(6));
        mvprintw(25, 30, "90");
        attroff(COLOR_PAIR(6));

        drawPixelHEX(25, 7, 0xffd700);
        drawPixelHEX(25, 8, 0xffd700);
        drawPixelHEX(25, 9, 0xffd700);
        drawPixelHEX(25, 10, 0xffd700);
        drawPixelHEX(25, 11, 0xffd700);
        drawPixelHEX(25, 12, 0xffd700);
        drawPixelHEX(25, 13, 0x333333);

        // END


        refresh();
        //makes the program stop for 33ms for us to get 30 FPS.
        napms(33);
    }

    nodelay(stdscr, FALSE);
}

