#pragma once

#define CAMERAS 4

#define GLITCH_PIXELS 10

enum Place {
	NONE,
	IN_VENT_LEFT,
	IN_VENT_RIGHT,
	VENT_LEFT,
	VENT_RIGHT,
	PLAYGROUND,
	LIVINGROOM,
	HALLWAY
} typedef Place;


struct Monster {
	Place targetPlace;
} typedef Monster;


struct Camera {
	char* name;
	int* image;
	int y;
	int x;
	int width;
	int height;

} typedef Camera;


void cameraWindow(int* radio, int* radioTimer, int FPS);

void drawCameraMiniMap(int camera, Camera* cameras);


