#pragma once

struct Data {
	int day;
	int hour;
	int hourDelay;
	int battery;
	int radio;

	int wins;
	int losses;
} typedef Data;



void loadData(Data* data);

void saveData(Data data);
