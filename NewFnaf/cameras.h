#pragma once

#define CAMERAS 4
#define MONSTERS 4

#define GLITCH_PIXELS 10



enum MonsterType {
	NONE,
	VENT,
	PLAYGROUND,
} typedef MonsterType;

struct Monster {
	MonsterType type;
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

void monstersTick(Monster* monsters, bool* resetScreen);

void cameraWindow(int* radio, int* radioTimer, long *time, int FPS, Monster* monsters);

void drawCameraMiniMap(int camera, Camera* cameras);




