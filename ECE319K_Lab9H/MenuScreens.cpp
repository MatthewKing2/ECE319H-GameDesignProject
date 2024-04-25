/*
 * MenuScreens.cpp
 *
 *  Created on: Apr 19, 2024
 *      Author: simon
 */

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/SlidePot.h"
#include "../inc/DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "images/images.h"
#include "MyButtons.h"
#include "MySounds.h"
//#include "lights.h"

#include "../inc/UART.h"

extern int sound; //this is the global for sounds that will be accessed in MySounds
                      //0 -> dash
                      //1 -> footsteps
                      //2 -> shoot
                      //3 -> click (menu button)
extern bool soundStop;


// Refreshes the start screen with the most up to data user input
void refreshStart(int currentChoice, bool spanish){
    int xt;
    char wordarr[2][4][25] = {{"Start ", "English", "Murder Mystery"}, {"Inicio", "Espanol", "misterio de asesinato"}};
    if(currentChoice == 0){
            xt = 50;
            for(int i = 0; i < 6; i++){
                ST7735_DrawCharS(xt, 74, wordarr[spanish][0][i], ST7735_CYAN, ST7735_CYAN, 2);
                xt+=12;
                }
            xt = 60;
            for(int i = 0; i < 7; i++){
                ST7735_DrawCharS(xt, 94, wordarr[0][1][i], ST7735_WHITE, ST7735_WHITE, 1);
                xt+=6;
            }
            xt = 60;
            for(int i = 0; i < 7; i++){
                ST7735_DrawCharS(xt, 108, wordarr[1][1][i], ST7735_WHITE, ST7735_WHITE, 1);
                xt+=6;
            }
        }
        else if(currentChoice == 1){
            xt = 50;
            for(int i = 0; i < 6; i++){
                ST7735_DrawCharS(xt, 74, wordarr[spanish][0][i], ST7735_WHITE, ST7735_WHITE, 2);
                xt+=12;
            }
            xt = 60;
            for(int i = 0; i < 7; i++){
                ST7735_DrawCharS(xt, 94, wordarr[0][1][i], ST7735_CYAN, ST7735_CYAN, 1);
                xt+=6;
            }
            xt = 60;
            for(int i = 0; i < 7; i++){
                ST7735_DrawCharS(xt, 108, wordarr[1][1][i], ST7735_WHITE, ST7735_WHITE, 1);
                xt+=6;
            }
        }
        else{
            xt = 50;
            for(int i = 0; i < 6; i++){
                ST7735_DrawCharS(xt, 74, wordarr[spanish][0][i], ST7735_WHITE, ST7735_WHITE, 2);
                xt+=12;
            }
            xt = 60;
            for(int i = 0; i < 7; i++){
                ST7735_DrawCharS(xt, 94, wordarr[0][1][i], ST7735_WHITE, ST7735_WHITE, 1);
                xt+=6;
            }
            xt = 60;
            for(int i = 0; i < 7; i++){
                ST7735_DrawCharS(xt, 108, wordarr[1][1][i], ST7735_CYAN, ST7735_CYAN, 1);
                xt+=6;
            }
        }

}

// Starts the Menu screens (exits when player starts game)
void startMenu(bool* spanish){
    Buttons b1;
    uint32_t up1 = 0;
    uint32_t down1 = 0;
    uint32_t left1 = 0;
    uint32_t right1 = 0;
    uint32_t dash1 = 0;
    uint32_t* up = &up1;
    uint32_t* down = &down1;
    uint32_t* left = &left1;
    uint32_t* right = &right1;
    uint32_t* dash = &dash1;
    b1.Buttons_Init();

    int currentChoice = 0;  // 0->start  1->english  2->spanish

    ST7735_SetRotation(0);
    ST7735_DrawBitmap(0, 160, start, 128, 160);
    ST7735_SetRotation(3);
    refreshStart(currentChoice, *spanish);

    while(1){

        // Read in the button press 
            // This function has delay built in
        b1.Menu_Buttons_In(up, down, left, right, dash);


        // Start of Game, exits function 
        if(*dash && currentChoice == 0){
            sound = 0;
            Sound_Start();
            return;
        }

        // Dont know
        else if(*right && currentChoice < 2){
            currentChoice++;
            sound = 2;
            Sound_Start();
            refreshStart(currentChoice, *spanish);
        }

        // Dont know 
        else if(*left && currentChoice > 0){
            currentChoice--;
            sound = 3;
            Sound_Start();
            refreshStart(currentChoice, *spanish);
        }

        // Dont know 
        else if(currentChoice == 1 && *dash && *spanish){
            *spanish = false;
            ST7735_SetRotation(0);
            ST7735_DrawBitmap(0, 160, start, 128, 160);
            ST7735_SetRotation(3);
            sound = 0;
            Sound_Start();
            refreshStart(currentChoice, *spanish);
        }

        // Dont know 
        else if(currentChoice == 2 && *dash && !*spanish){
            *spanish = true;
            ST7735_SetRotation(0);
            ST7735_DrawBitmap(0, 160, start, 128, 160);
            ST7735_SetRotation(3);
            sound = 1;
            Sound_Start();
            refreshStart(currentChoice, *spanish);
        }

    // Clear Button Presses 
    *up = 0;
    *down = 0;
    *left = 0;
    *right = 0;
    *dash = 0;


}

}


// Displays the dealth screen in English or Spanish
void deathScreen(bool spanish){
    if(spanish){
        ST7735_DrawBitmap(20, 90, diedspa, 118, 48);
    }
    else{
        ST7735_DrawBitmap(20, 90, diedeng, 123, 49);
    }
}

// Displays the End Screen
void endScreen(/*Player p1, Enemy e1, Enemy e2, Enemy e3,*/ bool spanish){
    ST7735_FillScreen(0x0000);   // set screen to black
    int xt = 5;
    char wordarr[2][4][9] = { {"First:  ", "Second: ", "Third:  ", "Hunter: "}, {"Primero:", "Segundo:", "Tercero:", "Cazador:"} };
    for(int i = 0; i < 8; i++){
        ST7735_DrawCharS(xt, 30, wordarr[spanish][0][i], ST7735_WHITE, ST7735_WHITE, 2);
        ST7735_DrawCharS(xt, 50, wordarr[spanish][1][i], ST7735_WHITE, ST7735_WHITE, 2);
        ST7735_DrawCharS(xt, 70, wordarr[spanish][2][i], ST7735_WHITE, ST7735_WHITE, 2);
        ST7735_DrawCharS(xt, 90, wordarr[spanish][3][i], ST7735_RED, ST7735_RED, 2);
        xt+=12;
    }
//    ST7735_DrawBitmap(90/*double check*/, 32 + (20*p1.rank), p1.image, 118, 48);
//    ST7735_DrawBitmap(90/*double check*/, 32 + (20*e1.rank), p1.image, 118, 48);
//    ST7735_DrawBitmap(90/*double check*/, 32 + (20*e2.rank), p1.image, 118, 48);
//    ST7735_DrawBitmap(90/*double check*/, 32 + (20*e3.rank), p1.image, 118, 48);
}







