#include <curses.h>
#include "draw.h"



void drawArt(int y_offcet, int x_offcet, int width, int height, int colorpair, int *art) {

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++)
        {

            int value = art[y * width + x];

            if (value == 1) {
                drawPixel(y + y_offcet, x + x_offcet, COLOR_PAIR(colorpair));
            }
            else {
                mvprintw(y + y_offcet, (x+ x_offcet)*2, "  ");
            }
        }
    }
}


void drawPixel(int y, int x, int colorpair) {
    attron(colorpair);
    mvaddch(y, x*2, ACS_BLOCK);
    mvaddch(y, x*2 + 1, ACS_BLOCK);
    attroff(colorpair);
}


