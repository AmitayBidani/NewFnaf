#include <curses.h>
#include "draw.h"

void drawArt(int y_offcet, int x_offcet, int width, int height, int colorpair, int *art) {

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++)
        {

            int value = art[y * width + x];

            if (value == 1) {
                drawPixel(y + y_offcet, x + x_offcet, COLOR_PAIR(colorpair));
            }
            else {
                mvprintw(y + y_offcet, (x+ x_offcet)*2, "  ");
            }
        }
    }
}


void drawPixel(int y, int x, int colorpair) {
    attron(colorpair);
    mvaddch(y, x*2, ACS_BLOCK);
    mvaddch(y, x*2 + 1, ACS_BLOCK);
    attroff(colorpair);
}

void drawPixelHEX(int y, int x, int hex) {

    int r, g, b;
    HexToRGB(hex, &r, &g, &b);
    int color = getColor(r, g, b);

    int colorPair = (color - 16) + 20;

    init_pair(colorPair, color, COLOR_BLACK);
    drawPixel(y, x, COLOR_PAIR(colorPair));

}

void HexToRGB(int hex, int *r,int *g,int *b) {
    /* >> moves 1 bit forward || &0xFF gets the last 2 digits in the hex
    * and in that way we can get a hex number that represent rgb colors: 0xRRGGBB
    * to an actual numbers we can work with.
    */ 

    *r = (hex >> 16) & 0xFF;
    *g = (hex >> 8) & 0xFF;
    *b = hex & 0xFF;
}

int getColor(int red, int green, int blue) {

    // if its grey we 
    if (red == green && green == blue) {
        if (red < 8) return 16;
        if (red > 238) return 231;
        return 232 + (red - 8) / 10;
    }

    //converting 0-255 to 0-5
    int r = (red*5) / 255;
    int g = (green*5) / 255;
    int b = (blue*5) / 255;

    //16- are the default colors so u must add 16
    return 16 + r * 36 + g * 6 + b;
}

void drawImage(int xOffcet, int yOffcet, int width, int height, int *image) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++)
        {

            drawPixelHEX(y + yOffcet, x + xOffcet, image[y * width + x]);

        }
    }
}

//for testing
void drawPalette(void) {
    int levels[6] = { 0, 95, 135, 175, 215, 255 };

    mvprintw(1, 2, "RGB cube: rows = red, groups = green, inside group = blue");

    for (int r = 0; r < 6; r++) {
        for (int g = 0; g < 6; g++) {
            for (int b = 0; b < 6; b++) {
                int hex = (levels[r] << 16) | (levels[g] << 8) | levels[b];
                int y = 3 + r;
                int x = 2 + g * 7 + b;
                drawPixelHEX(y, x, hex);
            }
        }
    }

    mvprintw(11, 2, "Grayscale");

    for (int i = 0; i < 24; i++) {
        int gray = 8 + i * 10;
        int hex = (gray << 16) | (gray << 8) | gray;
        drawPixelHEX(13, 2 + i, hex);
    }

    mvprintw(15, 2, "Basic test colors");
    drawPixelHEX(17, 2, 0xFF0000);
    drawPixelHEX(17, 3, 0x00FF00);
    drawPixelHEX(17, 4, 0x0000FF);
    drawPixelHEX(17, 5, 0xFFFF00);
    drawPixelHEX(17, 6, 0x00FFFF);
    drawPixelHEX(17, 7, 0xFF00FF);
    drawPixelHEX(17, 8, 0xFFFFFF);
}



