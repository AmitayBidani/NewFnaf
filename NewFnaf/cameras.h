#pragma once

#define CAMERAS 4
#define MONSTERS 3

#define GLITCH_PIXELS 10


struct Monster {
	int stage;
	int* image;
	int avgTime; // The average time it takes to the monster to go to the next stage
	int currentTime;

} typedef Monster;

struct Camera {
	char* name;
	int* image;
	int y;
	int x;
	int width;
	int height;

} typedef Camera;

int random(int min, int max);

void monstersTick(Monster* monsters, bool* resetScreen, int showTime, bool* keepRunning);

void cameraWindow(int* radio, int* radioTimer, long *time, int FPS, Monster* monsters);

void drawCameraMiniMap(int camera, Camera* cameras);




