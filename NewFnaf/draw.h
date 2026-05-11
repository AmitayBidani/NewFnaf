#pragma once

void initHexColors();

void drawPixel(int y, int x, int colorpair);

void drawArt(int y_offcet, int x_offcet, int width, int height, int colorpair, int*art);

void drawPixelHEX(int y, int x, int hex);

void HexToRGB(int hex, int* r, int* g, int* b);

int getColor(int red, int green, int blue);

void drawImage(int xOffcet, int yOffcet, int width, int height, int* image);
