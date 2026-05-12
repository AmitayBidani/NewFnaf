#include <curses.h>
#include <stdbool.h>
#include "draw.h"
#include "images.h"
#include "game.h"
void showGame() {
    nodelay(stdscr, TRUE);

    init_pair(5, getColor(255, 255, 255), getColor(29, 29, 29));
    init_pair(6, getColor(255, 215, 0), getColor(29, 29, 29));
    bool start = FALSE;


    while (1) {
        clear();
        char key;

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

        //Show Mask
        if (start) {
            drawImage(0, 0, MASK_WIDTH, MASK_HEIGHT, mask_pixels);
        }

        //drawImage(0, 0, MASK_WIDTH, MASK_HEIGHT, mask_pixels);

        //Show Light
        //drawImage(25, 8, LIGHT_WIDTH, LIGHT_HEIGHT, light_pixels);

        drawPixelHEX(25, 7, 0xffd700);
        drawPixelHEX(25, 8, 0xffd700);
        drawPixelHEX(25, 9, 0xffd700);
        drawPixelHEX(25, 10, 0xffd700);
        drawPixelHEX(25, 11, 0xffd700);
        drawPixelHEX(25, 12, 0xffd700);
        drawPixelHEX(25, 13, 0x333333);

        // END

        //shahar!!
        key = getch();

        switch (key) {
        case 'Q':
        case 'q':
            return;
            break;
        case 'm':
        case 'M':
            start = !start;
            break;

        }

    }

    nodelay(stdscr, FALSE);
    
}


