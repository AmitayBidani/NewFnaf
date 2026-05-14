
#include <curses.h>
#include <stdbool.h>

#include "draw.h"
#include "images.h"
#include "cameras.h"

void cameraWindow(int *radio, int *radioTimer, int FPS) {

    int key = 0;
    int camera = 0;

    Camera cameras[CAMERAS] = {
        {"Playground", camera_playground},
        {"Left Vent", camera_vent},
        {"Right Vent", camera_vent},
        {"Livingroom", camera_timer}
    };

	while (1) {

        (*radioTimer)++;
        if (*radioTimer >= FPS) {
            *radioTimer = 0;

            if(*radio > 0)
                (*radio)--;
        }

        if (key != -1) {
            erase();
            drawImage(0, 0, CAMERA_WIDTH, CAMERA_HEIGHT, cameras[camera].image);
            attron(COLOR_PAIR(5));
            mvprintw(21, 10, "Camera: %d, %s", camera + 1, cameras[camera].name);
            attroff(COLOR_PAIR(5));

            mvprintw(25, 2, "%d", *radio);

            attron(COLOR_PAIR(7));
            mvprintw(29, 2, "LEFT ARROW - Go left     RIGHT ARROW - Go right     C - Go back");
            attroff(COLOR_PAIR(7));
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

            }
        }
        napms(1000 / FPS);
        
	}
}