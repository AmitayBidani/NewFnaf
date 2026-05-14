#pragma once


#define BACKGROUND_WIDTH 60
#define BACKGROUND_HEIGHT 30

int background_pixels[BACKGROUND_WIDTH * BACKGROUND_HEIGHT];

#define MASK_WIDTH 60
#define MASK_HEIGHT 23

int mask_pixels[MASK_WIDTH * MASK_HEIGHT];

#define LIGHT_WIDTH 12
#define LIGHT_HEIGHT 10

int light_pixels[LIGHT_WIDTH * LIGHT_HEIGHT];

#define MENU_WIDTH 60
#define MENU_HEIGHT 30

int menu_pixels[MENU_WIDTH * MENU_HEIGHT];

#define CAMERA_WIDTH 60
#define CAMERA_HEIGHT 23

int camera_timer[CAMERA_WIDTH * CAMERA_HEIGHT];

int camera_playground[CAMERA_WIDTH * CAMERA_HEIGHT];

int camera_vent[CAMERA_WIDTH * CAMERA_HEIGHT];