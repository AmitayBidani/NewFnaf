#include <curses.h>
#include <stdbool.h>


#include "draw.h"
#include "images.h"

#include "cameras.h"
#include "game.h"

static int FPS = 30;

void showGame() {
    nodelay(stdscr, TRUE);

    init_pair(5, getColor(255, 255, 255), getColor(29, 29, 29));
    init_pair(6, getColor(255, 215, 0), getColor(29, 29, 29));
    init_pair(7, getColor(255, 255, 255), getColor(19, 19, 19));
    init_pair(8, COLOR_YELLOW, COLOR_BLACK);


    bool mask = false;
    bool light = false;

    bool keyLocked = false;

    bool resetScreen = true;

    int battery = 100;
    int batteryTimer = 0;
    int batteryTimer1 = 0;
    int key = 0;

    int radio = 100;
    int radioTimer = 0;
    


    Scene scene = MAIN_GAME;
    

    while (1) {

        radioTimer++;
        if (radioTimer >= FPS) {
            radioTimer = 0;
            if(radio > 0)
                radio--;
        }
        

        if (light) {
            batteryTimer++;
            if (batteryTimer >= FPS*2) {
                battery--;
                batteryTimer = 0;
                resetScreen = true;
                if (battery <= 0)
                    light = false;
            }
        }

        
        // START

        if (scene == MAIN_GAME) {
            if (resetScreen) {
                if (resetScreen) resetScreen = false;

                erase();
                drawImage(0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, background_pixels);

                attron(COLOR_PAIR(5));
                mvprintw(25, 5, "Battery:");


                mvprintw(27, 5, "Controls: ");
                mvprintw(27, 5, "C - Open camera   L - Use Flashlight   M - Toggle Mask ");
                mvprintw(28, 5, "Q - Quit");

                attroff(COLOR_PAIR(5));
                mvprintw(29, 5, "%d", radio);


                attron(COLOR_PAIR(6));
                mvprintw(25, 30, "%d", battery);
                attroff(COLOR_PAIR(6));

                if (mask) {
                    drawImage(0, 0, MASK_WIDTH, MASK_HEIGHT, mask_pixels);
                }

                if (battery > 0 && light) {
                    drawImage(25, 8, LIGHT_WIDTH, LIGHT_HEIGHT, light_pixels);
                }
                

                
                
                drawBar(7, 25, 6, 1,100,0,battery,0xffff00,0x000000);
                

                mvprintw(0, 0, "%d", key);

                refresh();
            }
        }
        else if (scene == CAMERA) {
            scene = MAIN_GAME;
            cameraWindow(&radio, &radioTimer, FPS);
            resetScreen = true;
            
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
        

        if (key != ERR) {
            resetScreen = true;
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
                break;
            case 'C':
            case 'c':
                scene = CAMERA;
                break;
            }

        }
        
        napms(1000 / FPS);
    }
    nodelay(stdscr, FALSE);
    
}


