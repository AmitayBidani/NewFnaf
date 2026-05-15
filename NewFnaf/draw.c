#include <curses.h>
#include "draw.h"


//Register all the colors from numbers as an pair we can use.
void initHexColors() {
    for (int color = 16; color <= 255; color++) {
        int colorPair = (color - 16) + 20;
        init_pair(colorPair, color, COLOR_BLACK);
    }
}


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

    if (hex != -1) {
        int r, g, b;
        HexToRGB(hex, &r, &g, &b);
        int color = getColor(r, g, b);

        int colorPair = (color - 16) + 20;

        drawPixel(y, x, COLOR_PAIR(colorPair));
    }
}

void HexToRGB(int hex, int *r,int *g,int *b) {
    /* >> moves (the number you chose) bits forward || &0xFF gets the last 2 digits in the hex
    * and in that way we can get a hex number that represent rgb colors: 0xRRGGBB
    * to an actual numbers we can work with.
    */ 
    *r = (hex >> 4 * 4) & 0xFF;
    *g = (hex >> 4 * 2) & 0xFF;
    *b = hex & 0xFF;
    
}

int getColor(int red, int green, int blue) {

    //we are converting the RGB number from 0-255 range for every color to 0-5 range.

    // detect if we entered a grey color
    if (red == green && green == blue) {
        //if its a really dark grey we will just use black in index 16.
        if (red < 8) return 16;
        //if its a really light grey we will just use white in index 231
        if (red > 238) return 231;
        //if its not black or white we will use the pallete between 232-255, so we have 24 colors of grey.
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

//this function is uesed to draw our battery bar    
void drawBar(int x_0, int y_0, int width, int height, int max , int min , int now ,int color_in , int color_out) {
   
    int box = (max - min) / width;
    int bar = now / box;
    for (int y = 0; y < height; y++) {
        for (int x=0; x< width; x++) 
        {
            if (x <= bar) {
                drawPixelHEX(y + y_0, x_0 + x, color_in);
            }
            else {
                drawPixelHEX(y + y_0, x_0 + x, color_out);
            }
            
        }
        
    }
    
    
}