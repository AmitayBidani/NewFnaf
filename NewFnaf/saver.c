#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "saver.h"


//Load the data from the file we saved/ or create it.
void loadData(Data* data) {

	char path[300];
	//Get the dir of: C:\Users\[Computer]\AppData\Local
	char* localDir = getenv("LOCALAPPDATA");

	if (localDir == NULL)
		return;

	strcpy(path, localDir);

	strcat(path, "\\FNAH");
	_mkdir(path);
	strcat(path, "\\save.dat");

	FILE* file = fopen(path, "rb");

	if (file != NULL) {
		fread(data, sizeof(Data), 1, file);
	}
	else {
		*data = (Data){0,0,0,100,110,0,0};
	}
}


//Save the data and change the file  
void saveData(Data data) {
	char path[300];

	//Get the dir of: C:\Users\[Computer]\AppData\Local
	char* localDir = getenv("LOCALAPPDATA");

	if (localDir == NULL)
		return;

	strcpy(path, localDir);

	strcat(path, "\\FNAH");
	_mkdir(path);
	strcat(path, "\\save.dat");

	FILE* file = fopen(path, "wb");

	if (file != NULL) {
		fwrite(&data, sizeof(data), 1, file);
		fclose(file);
	}
}