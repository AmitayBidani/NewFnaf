#include <curses.h>
#include <stdbool.h>
#include <windows.h>

#include "draw.h"
#include "images.h"

#include "saver.h"

#include "cameras.h"
#include "game.h"

static int FPS = 30;

static Data d;


//Save Before Leaving (When pressing the exit button in windows)
BOOL WINAPI consoleHandler(DWORD ctrlType) {
    saveData(d);
    return TRUE;
}

void showGame(Data data) {
    nodelay(stdscr, TRUE);
    SetConsoleCtrlHandler(consoleHandler, true);

    d = data;

    atexit(exit);

    init_pair(5, getColor(255, 255, 255), getColor(29, 29, 29));
    init_pair(6, getColor(255, 215, 0), getColor(29, 29, 29));
    init_pair(7, getColor(255, 255, 255), getColor(8, 8, 8));
    init_pair(8, getColor(200, 200, 200), getColor(8, 8, 8));

    bool mask = false;
    bool light = false;

    bool resetScreen = true;

    int *battery = &d.battery;
    int batteryTimer = 0;
    int key = 0;

    int radioTimer = 0;
    int *radio = &d.radio;

    int lightDelay = 0;
    int maskDelay = 0;
    
    Scene scene = MAIN_GAME;
    
    int hourDelay = 0;
    int *currentTime = &d.hour;
    int *currentDay = &d.day;
    Day days[5];

    
    for (int i = 0; i < 5; i++)
    {
        int value[3];
        for (int j = 0; j < 3; j++)
        {
            value[j] = random(25 - (i * 5), 27 - (i * 5)) * FPS;
        }

        days[i] = (Day){value[0], value[1], value[2], FPS * 2 - (i * 6)};
    }
    

    Monster monsters[3] = {
        {-1, blue_character, days[*currentDay].blueDelay, 0}, // camera 0 enemy
        {-1, orange_character, days[*currentDay].orangeDelay, 0}, // camera 1 enemy ( left vent )
        {-1, purple_character, days[*currentDay].purpleDelay, 0} // camera 2 enemy ( right vent )
    };

    while (1) {

        hourDelay++;
        if (hourDelay >= FPS * HOURTIME) {
            (*currentTime)++;
            resetScreen = true;
            hourDelay = 0;
        }
        if (*currentTime >= 6) {
            *currentTime = 0;
            *radio = 110;
            *battery = 100;
            clear();
            mvprintw(14, 58, "6 AM");
            refresh();

            //WIN
            if(*currentDay >= 4) {
                *currentDay = 0;
                clear();
                drawImage(21, 4, TEXT_WIDTH, TEXT_HEIGHT, you_pixel, 1);
                drawImage(21, 11, TEXT_WIDTH, TEXT_HEIGHT, won_pixel, 1);
                mvprintw(20, 54, "PRESS Q TO LEAVE");
                d = (Data){ 0, 0, 100, 110, 0 };
                saveData(d);
                refresh();
                char k;
                while (1) {
                    k = getch();
                    if (k == 'Q' || k == 'q') {
                        
                        nodelay(stdscr, FALSE);
                        return;
                    }  
                }
            }
            else {
                (*currentDay)++;
                napms(2000);
                clear();
                mvprintw(14, 57, "DAY: %d", *currentDay);
                refresh();
                napms(800);
                mvprintw(14, 57, "DAY: %d", *currentDay + 1);
                refresh();
                napms(3000);
            }
        }


        radioTimer++;
        if (radioTimer >= 18 - (*currentDay * 3)) {
            radioTimer = 0;
            if(*radio > 0)
                (*radio)--;
        }
        

        if (light) {
            batteryTimer++;
            if (batteryTimer >= days[*currentDay].batterySpeed) {

                (*battery)--;
                batteryTimer = 0;

                resetScreen = true;
                if (*battery <= 0)
                    light = false;
            }
        }

        if (light) {
            lightDelay++;

            if (monsters[0].stage == 2) {

                monsters[0].currentTime++;

                if (lightDelay >= FPS * 0.5) {
                    monsters[0].currentTime = 0;
                    monsters[0].stage = -1;
                }
                
            }
        }
        if (mask) {

            maskDelay++;

            for (int i = 1; i < 3; i++)
            {
                if (monsters[i].stage == 2) {

                    monsters[i].currentTime++;

                    if (maskDelay >= FPS * 0.5) {
                        monsters[i].currentTime = 0;
                        monsters[i].stage = -1;
                    }
                }
                
            }
        }

        
        // START


        if (scene == MAIN_GAME) {

            if (resetScreen) {
                resetScreen = false;

                erase();

                

                drawImage(0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, background_pixels, 1);

                //Draw Mainhall Character
                if (monsters[0].stage == 2) {
                    drawImage(28, 9, BLUECHAR_WIDTH, BLUECHAR_HEIGHT, blue_character, 1);
                }

                //Draw Left & Right Vent Characters
                for (int i = 1; i < 3; i++) {
                    if (monsters[i].stage == 2)
                        drawImage(5 + 45*(i-1), 14, VENTCHAR_WIDTH, VENTCHAR_HEIGHT, monsters[i].image, 1);
                }

                attron(COLOR_PAIR(5));
                mvprintw(25, 5, "Battery:");

                mvprintw(0, 0, "Day: %d", *currentDay +1);
                if (*currentTime == 0)
                    mvprintw(1, 0, "Time: 12 AM");
                else
                    mvprintw(1, 0, "Time: %d AM", *currentTime);

                mvprintw(27, 5, "Controls: ");
                mvprintw(27, 5, "C - Open camera   L - Use Flashlight   M - Toggle Mask ");
                mvprintw(28, 5, "Q - Quit");


                attron(COLOR_PAIR(6));
                mvprintw(25, 25, "%d | %d", *battery, batteryTimer);
                attroff(COLOR_PAIR(6));

                drawBar(14, 25, 10, 1,110,0,*battery,0xffd700,0x000000);

                if (mask)
                    drawImage(0, 0, MASK_WIDTH, MASK_HEIGHT, mask_pixels, 1);

                if (light)
                    drawImage(25, 8, LIGHT_WIDTH, LIGHT_HEIGHT, light_pixels, 1);


                //DEV
                //mvprintw(0, 0, "%d", key);
                //mvprintw(0, 0, "HALLWAY: %d | %d || %d", monsters[0].stage, monsters[0].currentTime, monsters[0].avgTime);
                //mvprintw(1, 0, "LEFT: %d | %d || %d", monsters[1].stage, monsters[1].currentTime, monsters[1].avgTime);
                //mvprintw(2, 0, "RIGHT: %d | %d || %d", monsters[2].stage, monsters[2].currentTime, monsters[2].avgTime);
                //

                refresh();
            }
        }
        else if (scene == CAMERA) {
            scene = MAIN_GAME;
            long time = 0;

            cameraWindow(radio, &radioTimer, &time, FPS, *currentDay, monsters, &(*currentTime), &hourDelay);

            //after we closed the camera window - left the camera loop
            int batterySpeed = days[*currentDay].batterySpeed+FPS;

            (*battery) -= (int)((time+batteryTimer) / (batterySpeed));
            if (*battery < 0) battery = 0;
            batteryTimer = (time + batteryTimer) % (batterySpeed);
                
            resetScreen = true;
            
        }

        bool keepRunning = true;
        monstersTick(monsters, &resetScreen, FPS * 3.5 - (*currentDay * 15), &keepRunning);

        //END GAME - LOSE
        if (!keepRunning || *radio <= 0) {
            
            clear();
            drawImage(20, 4, TEXT_WIDTH, TEXT_HEIGHT, you_pixel, 1);
            drawImage(20, 11, TEXT_WIDTH, TEXT_HEIGHT, lost_pixel, 1);
            d = (Data){ *currentDay, 0, 100, 110, 0 };
            saveData(d);

            if (*currentTime == 0)
                mvprintw(29, 51, "DAY: %d | Time: 12 AM", *currentDay + 1);
            else
                mvprintw(29, 51, "DAY: %d | Time: %d AM", *currentDay + 1, *currentTime);

            if(!keepRunning)
                mvprintw(19, 53, "A Monster Ate You!");
            else
                mvprintw(19, 41, "The Timer in The Livingroom Has Runned out!");

            mvprintw(20, 54, "PRESS Q TO LEAVE");
            refresh();
            char k;
            while (1) {
                k = getch();
                if (k == 'Q' || k == 'q') {
                    nodelay(stdscr, FALSE);
                    return;
                }

            }
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
                saveData(d);
                nodelay(stdscr, FALSE);
                return;
                break;
            case 'M':
            case 'm':
                mask = !mask;
                lightDelay = 0;
                maskDelay = 0;
                light = false;
                break;
            case 'L':
            case 'l':
                light = !light;
                lightDelay = 0;
                maskDelay = 0;
                mask = false;
                break;
            case 'C':
            case 'c':
                mask = false;
                light = false;
                lightDelay = 0;
                maskDelay = 0;
                scene = CAMERA;
                break;
            }
            
        }
        
        napms(1000 / FPS);
    }
    nodelay(stdscr, FALSE);
    
}


