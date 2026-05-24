#pragma once

#include "saver.h"

#define HOURTIME 30

void showGame(Data data);

struct Day {
	int blueDelay;
	int orangeDelay;
	int purpleDelay;
	int batterySpeed;
} typedef Day;


enum Scene{
	MAIN_GAME,
	CAMERA
} typedef Scene;

