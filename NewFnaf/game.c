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
    init_pair(8, getColor(200, 200, 200), getColor(19, 19, 19));

    bool mask = false;
    bool light = false;

    bool keyLocked = false;

    bool resetScreen = true;

    int battery = 100;
    int batteryTimer = 0;
    int key = 0;

    int radio = 110;
    int radioTimer = 0;
    
    Scene scene = MAIN_GAME;

    Monster monsters[3] = {
        {PLAYGROUND , -1, blue_character, 12*FPS, 0}, // camera 0 enemy
        {VENT, -1, orange_character, 15*FPS, 0}, // camera 1 enemy ( left vent )
        {VENT, -1, purple_character, 13*FPS, 0} // camera 2 enemy ( right vent )
    };

    while (1) {
        monstersTick(monsters, &resetScreen);

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


            if (monsters[0].stage == 2) {
                //Draw Mainhall Character
                drawImage(28, 9, BLUECHAR_WIDTH, BLUECHAR_HEIGHT, blue_character, 1);
            }
            if (monsters[1].stage == 2) {
                //Draw Left Vent Character
                drawImage(5, 14, VENTCHAR_WIDTH, VENTCHAR_HEIGHT, orange_character, 1);
            }
            if (monsters[2].stage == 2) {
                //Draw Right Vent Character
                drawImage(50, 14, VENTCHAR_WIDTH, VENTCHAR_HEIGHT, purple_character, 1);
            }


            if (resetScreen) {
                if (resetScreen) resetScreen = false;

                erase();
                drawImage(0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, background_pixels, 1);

                attron(COLOR_PAIR(5));
                mvprintw(25, 5, "Battery:");


                mvprintw(27, 5, "Controls: ");
                mvprintw(27, 5, "C - Open camera   L - Use Flashlight   M - Toggle Mask ");
                mvprintw(28, 5, "Q - Quit");


                attron(COLOR_PAIR(6));
                mvprintw(25, 25, "%d", battery);
                attroff(COLOR_PAIR(6));

                drawBar(14, 25, 10, 1,110,0,battery,0xffd700,0x000000);

                if (mask) {
                    drawImage(0, 0, MASK_WIDTH, MASK_HEIGHT, mask_pixels, 1);
                }

                if (battery > 0 && light) {
                    drawImage(25, 8, LIGHT_WIDTH, LIGHT_HEIGHT, light_pixels, 1);
                }

                mvprintw(0, 0, "%d", key);

                refresh();
            }
        }
        else if (scene == CAMERA) {
            scene = MAIN_GAME;
            long time = 0;

            cameraWindow(&radio, &radioTimer, &time, FPS, monsters);

            //after we closed the camera window - left the camera loop
            battery -= (int)(time / (4 * FPS));
            batteryTimer = time % (4 * FPS);
                
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
                mask = false;
                light = false;
                scene = CAMERA;
                break;
            }

        }
        
        napms(1000 / FPS);
    }
    nodelay(stdscr, FALSE);
    
}


