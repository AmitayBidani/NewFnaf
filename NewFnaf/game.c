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

    bool keyLocked = false;

    int battery = 100;
    int batteryTimer = 0;
    int key = 0;
    int keyCooldown = 0;


    while (1) {

        if (keyCooldown > 0)
            keyCooldown--;

        if (battery <= 0)
            light = false;

        if (light) {
            batteryTimer++;
            if (batteryTimer >= FPS*2) {
                battery--;
                batteryTimer = 0;
            }
        }

        // START

        if (key != -1 || batteryTimer == 0) {
            erase();
            drawImage(0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, background_pixels);

            attron(COLOR_PAIR(5));
            mvprintw(25, 5, "Battery:");


            mvprintw(27, 5, "Controls: ");
            mvprintw(27, 5, "C - Open camera   L - Use Flashlight   M - Toggle Mask ");
            mvprintw(28, 5, "Q - Quit");

            attroff(COLOR_PAIR(5));


            attron(COLOR_PAIR(6));
            mvprintw(25, 30, "%d", (int)battery);
            attroff(COLOR_PAIR(6));

            if (mask) {
                drawImage(0, 0, MASK_WIDTH, MASK_HEIGHT, mask_pixels);
            }

            if (battery > 0 && light) {
                drawImage(25, 8, LIGHT_WIDTH, LIGHT_HEIGHT, light_pixels);
            }

            drawPixelHEX(25, 7, 0xffd700);
            drawPixelHEX(25, 8, 0xffd700);
            drawPixelHEX(25, 9, 0xffd700);
            drawPixelHEX(25, 10, 0xffd700);
            drawPixelHEX(25, 11, 0xffd700);
            drawPixelHEX(25, 12, 0xffd700);
            drawPixelHEX(25, 13, 0xffd700);

            mvprintw(0, 0, "%d", key);

            refresh();
        }
        

        // END

        /*
        * For us to remove the delay after spamming a key, we need to get all the inputs
        * that we had before, and stop the loop when we stopped getting a key input in a row.
        */
        int ch;
        key = ERR;
        while ((ch = getch()) != ERR) {
            key = ch;
        }
        

        if (key != ERR && keyCooldown == 0) {
            switch (key) {
            case 'Q':
            case 'q':
                return;
                break;
            case 'M':
            case 'm':
                mask = !mask;
                light = false;
                break;
            case 'L':
            case 'l':
                light = !light;
                mask = false;

            }
        }
        
        napms(1000 / FPS);
    }
    nodelay(stdscr, FALSE);

    
    
}


