
#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>

#include "draw.h"
#include "images.h"
#include "cameras.h"
#include "game.h"
#include <time.h>

void cameraWindow(int *radio, int *radioTimer, long *time, int FPS, int day, Monster* monsters, int *currentTime, int *hourDelay) {


    int key = 0;
    int camera = 0;
    int glitchTimer = 0;

    bool resetScreen = true;

    Pixel glitchedPixels[GLITCH_PIXELS] = {0};

    Camera cameras[CAMERAS] = {
        {"Playground", camera_playground, 24, 40 + 50, 6, 2},
        {"Left Vent", camera_vent, 26, 36 + 50, 3, 3},
        {"Right Vent", camera_vent, 26, 47 + 50, 3, 3},
        {"Livingroom", camera_timer, 23, 32 + 50, 7, 2}
    };

	while (1) {

        bool keepRunning = true;
        monstersTick(monsters, &resetScreen, FPS * 4.5 - day*15, &keepRunning);
        if (!keepRunning)
            return;

        (*time)++;

        (*radioTimer)++;
        if (*radioTimer >= 22 - (day * 3)) {
            *radioTimer = 0;

            if(*radio > 0)
                (*radio)--;
        }
        (*hourDelay)++;
        if (*hourDelay >= FPS * HOURTIME) {
            (*currentTime)++;
            *hourDelay = 0;
        }
        if (*currentTime >= 6)
            return;


        if (radio <= 0)
            return;

        //ADD GLITCH EFFECT
        glitchTimer++;
        for (int i = 0; i < GLITCH_PIXELS; i++)
        {
            //fix back the last pixels that were glitched
            drawPixelHEX(glitchedPixels[i].y, glitchedPixels[i].x, glitchedPixels[i].hexColor);

            int x = random(0, 60);
            int y = random(0, 20);
            int width = CAMERA_WIDTH;
            glitchedPixels[i].x = x;
            glitchedPixels[i].y = y;
            glitchedPixels[i].hexColor = cameras[camera].image[y * width + x];

            //draw the glitch effect
            if(glitchTimer >= 3)
                drawPixelHEX(glitchedPixels[i].y, glitchedPixels[i].x, 0x252525);
        }
        

        if (resetScreen) {

            resetScreen = false;

            erase();

            drawImage(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT, cameras[camera].image ,1);
            attron(COLOR_PAIR(5));
            mvprintw(21, 10, "Camera: %d, %s", camera + 1, cameras[camera].name);
            attroff(COLOR_PAIR(5));

            //mvprintw(25, 2, "%d", *radio);

            attron(COLOR_PAIR(7));

            if (camera == 3)
                mvprintw(27, 12, "     TAB - Reset clock");

            mvprintw(27, 2, "C - Go back");
            mvprintw(28, 2, "Move Cameras: LEFT ARROW, RIGHT ARROW, UP ARROW, DOWN ARROW");
            
            attroff(COLOR_PAIR(7));

            drawCameraMiniMap(camera, cameras);
            
            //mvprintw(0, 0, "HALLWAY: %d | %d || %d", monsters[0].stage, monsters[0].currentTime, monsters[0].avgTime);
            //mvprintw(1, 0, "LEFT: %d | %d || %d", monsters[1].stage, monsters[1].currentTime, monsters[1].avgTime);
            //mvprintw(2, 0, "RIGHT: %d | %d || %d", monsters[2].stage, monsters[2].currentTime, monsters[2].avgTime);
            
        }

        //Draw the enemies in the right place

        for (int i = 1; i < 3; i++)
        {
            if (camera == i) {
                if (monsters[i].stage == 1)
                    drawImage(25 + 4*(i-1), 6- (i-1), VENTCHAR_WIDTH, VENTCHAR_HEIGHT, monsters[i].image, 1);
                else if (monsters[i].stage == 2)
                    drawImage(25 + 4*(i-1), 10, VENTCHAR_WIDTH, VENTCHAR_HEIGHT, monsters[i].image, 2);
            }
        }
        switch (camera) {
        case 0:
            if (monsters[0].stage == 0)
                drawImage(50, 9, BLUECHAR_WIDTH, BLUECHAR_HEIGHT, blue_character, 1);
            break;
        case 3:
            drawBar(68, 10, 22, 2, 110, 0, *radio, 0x87d7d7, 0xFFFFFF);
            break;
        }

        

        if (glitchTimer >= 3) {
            glitchTimer = 0;
            refresh();
        }

        

        int ch;
        key = ERR;
        while ((ch = getch()) != ERR) {
            key = ch;
        }


        if (key != ERR) {
            resetScreen = true;
            switch (key) {
            case 'C':
            case 'c':
                return;
                break;

            case KEY_LEFT:
                camera = (camera - 1 + CAMERAS) % CAMERAS;
                break;
            case KEY_RIGHT:
                camera = (camera + 1) % CAMERAS;
                break;
            case KEY_UP:
                if (camera == 1 || camera == 2)
                    camera = 0;
                else if (camera == 0)
                    camera = 3;
                break;
            case KEY_DOWN:
                camera = (camera + 1) % CAMERAS;
                break;


            case '\t':
                if(*radio < 110 && camera == 3)
                    (*radio)++;
                break;

            }
        }
        napms(1000 / FPS);
	}
}

void drawCameraMiniMap(int camera, Camera *cameras) {

    for (int c = 0; c < CAMERAS; c++)
    {
        for (int i = 0; i < cameras[c].width; i++) {
            for (int j = 0; j < cameras[c].height; j++) {

                if(camera == c)
                    drawHalfPixelHEX(cameras[c].y + j, cameras[c].x + i, 0x555555);
                else
                    drawHalfPixelHEX(cameras[c].y + j, cameras[c].x + i, 0x333333);
            }
        }
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 2; j++) {
            drawHalfPixelHEX(27+ j, 40 + 50 + i, 0x6dcafc);
        }
    }
    attron(COLOR_PAIR(8));

    mvprintw(27, 89, "▄");
    mvprintw(28, 89, "▀");

    mvprintw(27, 96, "▄");
    mvprintw(28, 96, "▀");

    mvaddch(26, 92, ACS_BLOCK);
    mvaddch(26, 93, ACS_BLOCK);
    
    attroff(COLOR_PAIR(8));

}

void monstersTick(Monster* monsters, bool* resetScreen, int showTime, bool* keepRunning) {

    int staged = -1;

    for (int i = 0; i < MONSTERS; i++)
    {

        //When he moves to the next stage
        if (monsters[i].currentTime == 0) {

            monsters[i].stage++;

            int avgTime = monsters[i].avgTime; 

            if (monsters[i].stage <= 0)
                monsters[i].currentTime = (int)(avgTime*0.75) + random((int)(avgTime * 0.3), avgTime);

            else if (monsters[i].stage == 1)
                monsters[i].currentTime = random((int)(avgTime / 3), (int)(avgTime / 1.5));

            else if (monsters[i].stage == 2)
                monsters[i].currentTime = showTime;

            else
                *keepRunning = false;
            
            if (staged == -1 && monsters[i].stage != 2 && monsters[i].stage > 0)
                staged = i;


            *resetScreen = true;
        }
        else
            monsters[i].currentTime--;

    }

    //If someone moved a to a new stage, we are delaying the others.
    if (staged != -1) {
        for (int i = 0; i < MONSTERS; i++)
        {
            if (staged != i && staged != -1 && monsters[i].stage < 2) {

                int avgTime = monsters[i].avgTime;
                monsters[i].currentTime += (int)(avgTime / 3) + random((int)(avgTime*0.35), (int)(avgTime*0.65));
            }
        }
    }
}

int random(int min, int max) {
    int value = rand() % (max - min + 1) + min;
    return value;
}