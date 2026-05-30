#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "draw.h"
#include "images.h"

#include "mainmenu.h"
#include "game.h"

#include "saver.h"



//Show the mainmenu screen
void showMainMenu() {

    Data data;
    loadData(&data);

    init_pair(1, getColor(255, 255, 255), getColor(8, 8, 8));
    init_pair(2, getColor(255,255,255), getColor(40,40,40));
    init_pair(3, getColor(255, 255, 255), getColor(8,8,8));
    init_pair(4, getColor(255, 255, 255), getColor(20, 20, 20));
    init_pair(5, getColor(255, 185, 64), getColor(20, 20, 20));
    init_pair(6, getColor(201, 200, 195), getColor(20, 20, 20));

    

    int selected = 0;
    int key = 0;
    int optionSize = 5;

    char** options;
    //Choose the right Menu options
    if (data.day == 0 && data.hour == 0 && data.battery == 100 && data.wins == 0 && data.radio == 110) 
        optionSize = 4;
    

    options = malloc(optionSize * sizeof(char[50]));
    if (options == NULL)
        return;

    int addValue = optionSize - 4;

    if(optionSize == 5)
        options[0] = "- Continue";
    options[0 + addValue] = "- New Game";
    options[1 + addValue] = "- How To Play";
    options[2 + addValue] = "- Credits";
    options[3 + addValue] = "- Quit";

    

    bool running = true;
    while (running) {

        if (key != -1) {
            erase();
            drawImage(0, 0, MENU_WIDTH, MENU_HEIGHT, menu_pixels, 1);
            
            if (optionSize == 5) {
                attron(COLOR_PAIR(1));
                mvprintw(15, 20, "(Day: %d)", data.day+1);
                attroff(COLOR_PAIR(1));
            }
            

            for (int i = 0; i < optionSize; i++)
            {
                if (i == selected) {
                    attron(COLOR_PAIR(2));
                    mvprintw(15 + i, 8, options[i]);
                    attroff(COLOR_PAIR(2));
                }
                else {
                    attron(COLOR_PAIR(1));
                    mvprintw(15 + i, 8, options[i]);
                    attroff(COLOR_PAIR(1));
                }
            }
            attron(COLOR_PAIR(3));
            mvprintw(29, 2, "Arrow Up - Up   Arrow Down - Down   Enter - Select");

            mvprintw(29, 110, "Wins: %d", data.wins);
            attroff(COLOR_PAIR(3));

            refresh();

            key = getch();



            switch (key) {
            case KEY_UP:
                selected = (selected - 1 + optionSize) % optionSize;
                break;
            case KEY_DOWN:
                selected = (selected + 1) % optionSize;
                break;
            case 10: // PRESSED ENTER
                running = false;
                break;
            }
        }
    }

    
    click(options[selected], data);

    free(options);
}


//Add a loading screen.
void loadScreen() {
    clear();
    wbkgd(stdscr, COLOR_PAIR(3));

    attron(COLOR_PAIR(3));
    mvprintw(15, 56, "Loading..");
    attroff(COLOR_PAIR(3));

    refresh();
}



//This function is running only when you press enter!
void click(char* selected, Data data) {

    if (strcmp(selected, "- Continue") == 0) {
        loadScreen();
        napms(700);
        showGame(data);
        
    }
    else if (strcmp(selected, "- New Game") == 0) {
        loadScreen();
        data = (Data){0,0,0,100,110,data.wins,0};
        napms(700);
        showGame(data);
    }
    else if (strcmp(selected, "- How To Play") == 0) {
        howToPlay();
    }
    else if (strcmp(selected, "- Credits") == 0) {
        Credits();
    }
    else if (strcmp(selected, "- Quit") == 0) {
        return;
    }

    showMainMenu();
}

void howToPlay() {
    char c;

    while (1) {
        clear();
        bkgd(COLOR_PAIR(4));

        mvprintw(4, 55, "HOW TO PLAY:");


        mvprintw(6, 1, "The Goal:");
        attron(COLOR_PAIR(6));
        mvprintw(7, 1, "In this game we have 5 nights, every night the game gets harder as you move on.");
        mvprintw(8, 1, "You start in 12 AM and the night ends in 6 AM.");
        attroff(COLOR_PAIR(6));
        mvprintw(10, 1, "What we must avoid:");

        mvprintw(11, 1, "- The Monsters:");
        mvprintw(16, 1, "- The Timer:");

        attron(COLOR_PAIR(6));
        mvprintw(12, 1, "     You need to defend yourself from the monsters coming from:");
        attron(COLOR_PAIR(5));
        mvprintw(13, 1, "                                                                      Avoid him using the flashlight ( PRESS: L )");
        mvprintw(14, 1, "                                                                      Avoid him using the mask       ( PRESS: M )");
        attroff(COLOR_PAIR(5));
        attron(COLOR_PAIR(6));
        mvprintw(13, 1, "     The Hallway (the middle): Arnavi                          |");
        mvprintw(14, 1, "     The Hallway (the middle): Ha-El Hazaov, Ha-Shed Hasagol   |");

        mvprintw(17, 1, "     And also the timer in the living room is running out and you must to reset it!");
        mvprintw(18, 1, "     To reset it you must access the camera, go to the living room using the arrow keys, and reset the timer!");
        attron(COLOR_PAIR(5));
        mvprintw(19, 1, "     Open camera: ( PRESS: C )  Reset timer:  ( PRESS: TAB )");
        attroff(COLOR_PAIR(5));
        attroff(COLOR_PAIR(6));
        
        mvprintw(21, 1, "The Camera:");
        attron(COLOR_PAIR(6));
        mvprintw(22, 1, "In the camera, you might notice that things might show or even disappear,");
        mvprintw(23, 1, "Those that appear in the vents eventually, you might see them before in the cameras,");
        mvprintw(24, 1, "And the one in the hallway you might see him changing his position.");
        attroff(COLOR_PAIR(6));


        mvprintw(29, 1, "PRESS Q TO GO BACK");

        refresh();

        c = getch();
        if (c == 'Q' || c == 'q') {
            return;
        }
    }
}

void Credits() {
    char c;

    while (1) {
        clear();

        bkgd(COLOR_PAIR(4));

        mvprintw(5, 56, "CREDITS:");

        
        mvprintw(8, 38, "- Shahar Jungmann - Developer & Art Director");
        mvprintw(9, 38, "- Amitay Bidani - Developer & Designer");
        
        attron(COLOR_PAIR(6));
        mvprintw(20, 36, "Developed as an project in HIT college.");
        mvprintw(21, 36, "Do not publish without a premmision of the creators.");
        attroff(COLOR_PAIR(6));

        mvprintw(29, 1, "PRESS Q TO GO BACK");


        refresh();

        c = getch();
        if (c == 'Q' || c == 'q') {
            return;
        }
    }
}


