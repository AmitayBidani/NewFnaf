
#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>

#include "draw.h"
#include "images.h"
#include "cameras.h"

void cameraWindow(int *radio, int *radioTimer, int FPS) {

    int key = 0;
    int camera = 0;
    int glitchTimer = 0;

    Pixel glitchedPixels[GLITCH_PIXELS] = {0};

    Camera cameras[CAMERAS] = {
        {"Playground", camera_playground, 24, 40 + 50, 6, 2},
        {"Left Vent", camera_vent, 26, 36 + 50, 3, 3},
        {"Right Vent", camera_vent, 26, 47 + 50, 3, 3},
        {"Livingroom", camera_timer, 23, 32 + 50, 7, 2}
    };

	while (1) {

        (*radioTimer)++;
        if (*radioTimer >= FPS) {
            *radioTimer = 0;

            if(*radio > 0)
                (*radio)--;
        }

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

        if (glitchTimer >= 3) glitchTimer = 0;
        refresh();
        

        if (key != -1) {
            erase();

            drawImage(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT, cameras[camera].image);
            attron(COLOR_PAIR(5));
            mvprintw(21, 10, "Camera: %d, %s", camera + 1, cameras[camera].name);
            attroff(COLOR_PAIR(5));

            mvprintw(25, 2, "%d", *radio);

            attron(COLOR_PAIR(7));

            if (camera == 3)
                mvprintw(27, 12, "     TAB - Reset clock");

            mvprintw(27, 2, "C - Go back");
            mvprintw(28, 2, "Move Cameras: LEFT ARROW, RIGHT ARROW, UP ARROW, DOWN ARROW");
            
            attroff(COLOR_PAIR(7));

            drawCameraMiniMap(camera, cameras);
            
            

            refresh();
        }

        

        int ch;
        key = ERR;
        while ((ch = getch()) != ERR) {
            key = ch;
        }


        if (key != ERR) {

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
                if(*radio < 100 && camera == 3)
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

int random(int min, int max) {
    return rand() % (max-min+1) + min;
}