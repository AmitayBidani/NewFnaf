#pragma once

struct Data {
	int day;
	int hour;
	int battery;
	int radio;
	int wins;
} typedef Data;



void loadData(Data* data);

void saveData(Data data);
