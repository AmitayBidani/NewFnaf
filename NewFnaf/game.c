#include <curses.h>
#include <stdbool.h>
#include <windows.h>

#include "draw.h"
#include "images.h"

#include "saver.h"

#include "cameras.h"
#include "game.h"

static Data d;


//Save Before Leaving (When pressing the exit button in windows)
bool quit() {
    saveData(d);
    return true;
}


//Show The Game Window
void showGame(Data data) {
    nodelay(stdscr, true);
    SetConsoleCtrlHandler(quit, true);

    d = data;
    
    atexit(exit);

    init_pair(5, getColor(255, 255, 255), getColor(28, 28, 28));
    init_pair(6, getColor(255, 215, 0), getColor(28, 28, 28));
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
    
    int *timeDelay = &d.hourDelay;
    int *currentTime = &d.hour;
    int *currentDay = &d.day;
    Day days[5];
    
    Message message = {0,NULL};
    
    for (int i = 0; i < 5; i++)
    {
        int value[3];
        for (int j = 0; j < 3; j++)
        {
            value[j] = random(20 - (i * 4), 22 - (i * 4)) * FPS;
        }

        days[i] = (Day){value[0], value[1], value[2], FPS - (i * 3) + 10};
    }
    

    Monster monsters[3] = {
        {-1, blue_character, days[*currentDay].blueDelay, 0}, // camera 0 enemy
        {-1, orange_character, days[*currentDay].orangeDelay, 0}, // camera 1 enemy ( left vent )
        {-1, purple_character, days[*currentDay].purpleDelay, 0} // camera 2 enemy ( right vent )
    };

    while (1) {

        (*timeDelay)++;
        if (*timeDelay >= FPS * HOURTIME) {
            (*currentTime)++;
            resetScreen = true;
            *timeDelay = 0;
        }

        if (message.time != 0 || message.text != NULL) {
            if (message.time <= 0) {
                message.text = NULL;
                resetScreen = true;
            }
            message.time--;
        }

        //IF TIME PASSED 5 (WENT TO 6 AM AND ENDED THE DAY)
        if (*currentTime >= 6) {
            *currentTime = 0;
            *radio = 110;
            *battery = 100;

            //reset monsters position
            monsters[0] = (Monster){ -1, blue_character, days[*currentDay].blueDelay, 0 }; // camera 0 enemy
            monsters[1] = (Monster){ -1, orange_character, days[*currentDay].orangeDelay, 0 }; // camera 1 enemy ( left vent )
            monsters[2] = (Monster){ -1, purple_character, days[*currentDay].purpleDelay, 0 }; // camera 2 enemy ( right vent )

            clear();
            mvprintw(14, 58, "6 AM");
            refresh();
            napms(2000);

            //WIN
            winMessage(&(*currentDay), &data);
            if(*currentDay == 0)
                return;
        }


        radioTimer++;
        if (radioTimer >= 18 - (*currentDay * 3)) {
            radioTimer = 0;
            if(*radio > 0)
                (*radio)--;
        }
        
        //DROP DOWN THE BATTERY WHEN USING THE LIGHT
        if (light) {
            if (*battery <= 0) {
                light = false;
                resetScreen = true;
            }
                
            else {
                batteryTimer++;

                if (batteryTimer >= (int)(days[*currentDay].batterySpeed / 8)) {

                    (*battery)--;
                    batteryTimer = 0;

                    resetScreen = true;

                }
            }

            
        }


        //CLEAR ENEMIES WITH THE LIGHT (WITH THE DELAY)
        if (light) {
            lightDelay++;

            if (monsters[0].stage == 2) {

                monsters[0].currentTime++;

                //Make sure you used the light at least 1/5 second to remove the monster.
                if (lightDelay >= FPS * CLEARMONSTER_TIME) {
                    monsters[0].currentTime = 0;
                    monsters[0].stage = -1;
                    resetScreen = true;
                }

            }
        }

        //CLEAR ENEMIES WITH THE MASK (WITH THE DELAY)
        if (mask) {
            maskDelay++;
            for (int i = 1; i < 3; i++)
            {
                if (monsters[i].stage == 2) {

                    monsters[i].currentTime++;
                    //Make sure you used the mask at least 1/5 second to remove the monster.
                    if (maskDelay >= FPS * CLEARMONSTER_TIME) {
                        monsters[i].currentTime = 0;
                        monsters[i].stage = -1;
                        resetScreen = true;
                    }
                }
                
            }
        }

        
        if (scene == MAIN_GAME) {


            //UPDATE GAME GRAPHICS
            if (resetScreen) {
                resetScreen = false;

                erase();
                drawImage(0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT, background_pixels, 1);

                //Draw Mainhall Character
                if (monsters[0].stage == 2) {
                    drawImage(27, 9, BLUECHAR_WIDTH, BLUECHAR_HEIGHT, blue_character, 1);
                }

                //Draw Left & Right Vent Characters
                for (int i = 1; i < 3; i++) {
                    if (monsters[i].stage == 2)
                        drawImage(5 + 45*(i-1), 14, VENTCHAR_WIDTH, VENTCHAR_HEIGHT, monsters[i].image, 1);
                }



                attron(COLOR_PAIR(5));

                mvprintw(0, 2, "Day: %d", *currentDay +1);

                if (*currentTime == 0)
                    mvprintw(1, 2, "Time: 12 AM");
                else
                    mvprintw(1, 2, "Time: %d AM", *currentTime);

                mvprintw(28, 5, "C - Open camera  L - Flashlight  M - Mask  Q - Quit");


                attron(COLOR_PAIR(6));
                mvprintw(27, 5, "Battery:");
                //mvprintw(27, 35, "%d | %d | %d", *battery, batteryTimer, light);

                if (message.time != 0 && message.text != NULL) {
                    mvprintw(27, 25, message.text);
                }

                attroff(COLOR_PAIR(6));

                drawBar(14, 27, 10, 1, 100, 0, *battery, 0xf5c925, 0x111111);

                if (mask)
                    drawImage(0, 0, MASK_WIDTH, MASK_HEIGHT, mask_pixels, 1);

                if (light)
                    drawImage(24, 8, LIGHT_WIDTH, LIGHT_HEIGHT, light_pixels, 1);


                


                //DEV
                //mvprintw(0, 0, "%d", key);
                //mvprintw(0, 0, "HALLWAY: %d | %d || %d", monsters[0].stage, monsters[0].currentTime, monsters[0].avgTime);
                //mvprintw(1, 0, "LEFT: %d | %d || %d", monsters[1].stage, monsters[1].currentTime, monsters[1].avgTime);
                //mvprintw(2, 0, "RIGHT: %d | %d || %d", monsters[2].stage, monsters[2].currentTime, monsters[2].avgTime);
                //

                refresh();
            }
        }

        //DRAW CAMERA IF OPENED
        else if (scene == CAMERA) {
            scene = MAIN_GAME;

            cameraWindow(radio, &radioTimer, &(*battery), &batteryTimer, days[*currentDay].batterySpeed, *currentDay, monsters, &(*currentTime), &(*timeDelay));

            resetScreen = true;
        }






        bool keepRunning = true;
        monstersTick(monsters, &resetScreen, FPS * 3.5 - (*currentDay * 15), &keepRunning);

        //END GAME - LOSE
        if (!keepRunning || *radio <= 0) {
            loseMessage(&(*currentTime), &(*currentDay), keepRunning);
            return;
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
                nodelay(stdscr, false);
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
                if (*battery > 0) {
                    light = !light;
                    lightDelay = 0;
                    maskDelay = 0;
                    mask = false;
                }
                else {
                    message = (Message){ 1 * FPS, "NO BATTERY LEFT!" };
                }
                
                
                break;
            case 'C':
            case 'c':
                if (*battery > 0) {
                    mask = false;
                    light = false;

                    lightDelay = 0;
                    maskDelay = 0;
                    scene = CAMERA;
                }
                else {
                    message = (Message){ 1 * FPS, "NO BATTERY LEFT!" };
                }
                
                break;
            }
            
        }
        
        napms(1000 / FPS);
    }
    nodelay(stdscr, false);
    
}


void winMessage(int *currentDay, Data* data) {
    if (*currentDay >= 4) {
        *currentDay = 0;
        clear();
        drawImage(21, 4, TEXT_WIDTH, TEXT_HEIGHT, you_pixel, 1);
        drawImage(21, 11, TEXT_WIDTH, TEXT_HEIGHT, won_pixel, 1);
        mvprintw(23, 54, "PRESS Q TO LEAVE");
        mvprintw(21, 54, "Total Losses: %d", data->losses);
        mvprintw(20, 54, "Win Number: %d !", data->wins + 1);
        d = (Data){ 0,0,0,100,110,d.wins + 1,0 };
        saveData(d);
        refresh();
        char k;
        while (1) {
            k = getch();
            if (k == 'Q' || k == 'q') {
                nodelay(stdscr, false);
                return;
            }
        }
    }
    else {
        (*currentDay)++;
        clear();
        mvprintw(14, 57, "DAY: %d", *currentDay);
        refresh();
        napms(800);
        mvprintw(14, 57, "DAY: %d", *currentDay + 1);
        refresh();
        napms(3000);
    }
}

void loseMessage(int* currentTime, int *currentDay, bool keepRunning) {
    clear();
    drawImage(20, 4, TEXT_WIDTH, TEXT_HEIGHT, you_pixel, 1);
    drawImage(20, 11, TEXT_WIDTH, TEXT_HEIGHT, lost_pixel, 1);


    if (*currentTime == 0)
        mvprintw(29, 52, "DAY: %d | Time: 12 AM", *currentDay + 1);
    else
        mvprintw(29, 52, "DAY: %d | Time: %d AM", *currentDay + 1, *currentTime);

    if (!keepRunning)
        mvprintw(19, 53, "A Monster Ate You!");
    else
        mvprintw(19, 41, "The Timer in The Livingroom Has Runned out!");

    d = (Data){ *currentDay,0,0,100,110,d.wins,d.losses + 1 };
    saveData(d);

    mvprintw(20, 54, "PRESS Q TO LEAVE");
    refresh();
    char k;
    while (1) {
        k = getch();
        if (k == 'Q' || k == 'q') {
            nodelay(stdscr, false);
            return;
        }

    }
}

