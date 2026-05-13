#include <curses.h>
#include <stdbool.h>
#include "draw.h"
#include "images.h"
#include "game.h"

static int FPS = 30;

void showGame() {
    nodelay(stdscr, TRUE);

    init_pair(5, getColor(255, 255, 255), getColor(29, 29, 29));
    init_pair(6, getColor(255, 215, 0), getColor(29, 29, 29));
    bool mask = false;
    bool light = false;

    int battery = 5000;

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
        mvprintw(25, 30,"%d",(int)(battery*2/100));
        attroff(COLOR_PAIR(6));

        if (mask) {
            drawImage(0, 0, MASK_WIDTH, MASK_HEIGHT, mask_pixels);
        }

        if (battery > 1) {
            if (light) {
                drawImage(25, 8, LIGHT_WIDTH, LIGHT_HEIGHT, light_pixels);
                battery--;
            }
        }

        drawPixelHEX(25, 7, 0xffd700);
        drawPixelHEX(25, 8, 0xffd700);
        drawPixelHEX(25, 9, 0xffd700);
        drawPixelHEX(25, 10, 0xffd700);
        drawPixelHEX(25, 11, 0xffd700);
        drawPixelHEX(25, 12, 0xffd700);
        drawPixelHEX(25, 13, 0xffd700);

        // END
        refresh();

        //shahar!!
        key = getch();

        switch (key) {
        case 'Q':
        case 'q':
            return;
            break;
        case 'm':
        case 'M':
            mask = !mask;
            light = false;
            break;
        case 'l':
        case 'L':
            light = !light;
            mask = false;

        }
        napms((int)(1000 / FPS));

    }
    

    nodelay(stdscr, FALSE);
    
}


