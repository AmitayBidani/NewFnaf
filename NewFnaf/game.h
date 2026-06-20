#pragma once

#include "saver.h"

#define HOURTIME 30
#define CLEARMONSTER_TIME 0.2
#define FPS 30

struct Day {
	int blueDelay;
	int orangeDelay;
	int purpleDelay;
	int batterySpeed;
} typedef Day;

struct Message {
	int time;
	char* text;
}  typedef Message;

enum Scene{
	MAIN_GAME,
	CAMERA
} typedef Scene;

void showGame(Data data);
void winMessage(int* currentDay, Data* data);
void loseMessage(int* currentTime, int* currentDay, bool keepRunning);