
//------------------------------------------------------------------------------
// File: Lab9HMain.cpp
// Author: Matthew King
// Description: 
//   This file contains the main implementation for Lab 9 ECE319H, running on 
//   the MSPM0G3507 microcontroller. It initializes the hardware, sets up the 
//   game environment, and contains the main game loop and interrupt handler 
//   for the game engine. 
//------------------------------------------------------------------------------

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
#include "Player.h"
#include "MyJoystick.h"
#include "../inc/ADC.h"   
#include "Frame.h"
#include "LCDisplay.h"
#include "LaserShot.h"
#include "Enemy.h"
#include "../inc/FIFO2.cpp"
#include "GameUart.h"
#include "MyButtons.h"
#include "MenuScreens.h"
#include "MySounds.h"
extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);


// Microcontroller Clock
void PLL_Init(void){ 
  Clock_Init80MHz(0);   // Set phase lock loop (PLL) to run at 80MHz
}

// TExaS Scope Pin Setup
uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}


//------------------------------------------------------------------------------
// Setup for Game (Global Variables)
//------------------------------------------------------------------------------
// Map and Enemies
Frame frames[9];          // Global Array of Frames 
Shot shots[4];            // Global Array of Shots
Enemy enemys[3];          // Global Array of Enemys
// Player
Player p1(60, 60, bobaFett, false);
uint32_t currentFrame = 0;  // Should be part of Player class
bool alive = true;          // Should be part of Player class
uint32_t health = 30;       // Should be part of Player class
// Sound 
int sound;
bool soundStop; 
// Controls
Buttons b1;
Joystick j1;
// Receiver (Uart)
Receiver r1;
uint32_t packets = 0;
// Display & Languages
LCD myDisplay;                  
bool s = false;
bool* spanish = &s;



//------------------------------------------------------------------------------
// Timer G12 Game Engine (ISR)
// Description: This ISR gets called at 30hz to update the state of the game, so
//              that the screen can display the new state at 30hz. 
// Order of Operations:
//  1) Is player Alive?
//  2) Delete old shots
//  3) Handle player exiting frame
//  4) Take player input
//  5) Move player
//  6) Handle Shooting
//  7) Transmit my information
//  8) Receive the enemies information
//  9) Set display ready flag
//  10) Repeat!
//------------------------------------------------------------------------------
void TIMG12_IRQHandler(void){

    // 1) Alive
    //-----------------------------------------------------------
    if(p1.touchingLaser(currentFrame)){
      if(health == 1){
        alive = false;
        greenOff();
        redOn();
      }else{
        health --;
      }
    } 

    // 2) Clear Shots 
    //-----------------------------------------------------------
      myDisplay.clearShots(currentFrame);

    // 3) Exit 
    //-----------------------------------------------------------
      bool exit = false;
      uint32_t newFrame;
      exit = p1.touchingExit(currentFrame, &newFrame);
      if(exit){
        myDisplay.FrameShift = true;
        myDisplay.oldFrame = currentFrame;
        myDisplay.newFrame = newFrame;
        currentFrame = newFrame;
        sound = 0;
        Sound_Start();
      }

    // 4) Read Joystick 
    //-----------------------------------------------------------
      uint32_t x = 0;
      uint32_t y = 0;
      j1.ADC_InDual(ADC1, &x, &y);

    // 5) Move Player
    //-----------------------------------------------------------
      p1.moveLinear(x, y, currentFrame);         

    // 6) Shoot 
    //-----------------------------------------------------------
      // Read buttons 
      uint32_t up;
      uint32_t down;
      uint32_t left;
      uint32_t right;
      uint32_t dash;
      b1.Buttons_In(&up, &down, &left, &right, &dash);
      bool shot = false;
      uint32_t shotDirection = 0;
      // Shoot down
      if(down && !dash && !left && !right){
        shots[0].generate(p1.x,p1.y, 1,currentFrame);
        shot = true;
        shotDirection = 1;
      }
      // Shoot left
      else if(left && !down && !dash && !right){
        shots[0].generate(p1.x,p1.y, 2,currentFrame);
        shot = true;
        shotDirection = 2;
      }
      // Shoot right
      else if(right && !dash && !down && !left){
        shots[0].generate(p1.x,p1.y, 3,currentFrame);
        shot = true;
        shotDirection = 3;
      }
      // Shot up (dash)
      else if(dash && !down && !right && !left){
        shots[0].generate(p1.x,p1.y, 0,currentFrame);
        shot = true;
        shotDirection = 0;
      }
      // Play the shooting sound when player shoots
      if(shot){
        sound = 2;
        Sound_Start();
      }

    // 7) Send my Info
    //-----------------------------------------------------------
    UART1_Transmit(1, currentFrame, alive, false, shot, shotDirection, p1.x, p1.y);

    // 8) Receive Enemy info (Read data out of SW Fifo)
    //-----------------------------------------------------------
    // 8.1) Get new data
    bool newData = false;
    char data1;
    char data2;
    char data3;
    char data4;
    data1 = UART2_InChar();
    uint32_t stopper = 0;
    while((data1 != ((1<<7)|1)) && stopper < 4){
      data1 = UART2_InChar();
      stopper ++;
    }
    if(stopper != 4){
      newData = true;  
      data2 = UART2_InChar();
      data3 = UART2_InChar();
      data4 = UART2_InChar();
      packets ++;
      r1.receiverTranslate(data1, data2, data3, data4);
      GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    }

    // 8.2) Update Enemies
    enemys[0].updatePosition(r1.x, r1.y, r1.frame, r1.alive);

    // 8.3) Update Shots
    if(r1.shot){
      shots[1].generate(enemys[0].x, enemys[0].y, r1.shotDirection, enemys[0].frame);
    }

    // 9) Set LCD Read Flag (so game screen can be udates)
    //-----------------------------------------------------------
    myDisplay.DisplayReady = true;

    // 10) Repeat!
    //-----------------------------------------------------------
}




//------------------------------------------------------------------------------
// Main Thread
// Description: Initlizes board, set up game map, displays the starting screen. 
//              Then the game loop is entered. The loop will check if the player 
//              has won, if the game is still in progress, and if the player has
//              lost. It will display the correct screen in all three cases.
//------------------------------------------------------------------------------
int main(){
  __disable_irq();
  PLL_Init(); 
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_SetRotation(3);
  uint16_t red = ST7735_Color565(255, 0, 0);

  // Set up each frame (room) of the gaem
  //-----------------------------------------------------------
  // Frame 0
  frames[0].InitWall(0, 0, 4, 128);         // Left Wall 
  frames[0].InitWall(124, 0, 128, 128);     // Right Wall 
  frames[0].InitWall(4, 0, 125, 4);         // Top Wall 
  frames[0].InitWall(4, 124, 125, 128);     // Bottom Wall 
  frames[0].InitWall(20, 20, 90, 24);       // Top  
  frames[0].InitWall(90, 70, 94, 100);      // Right Side   
  frames[0].InitExit(0,59,4,69, 111,60,1);  // Left Exit 
  frames[0].InitExit(59,124,69,128, 59,15,5); // Bottom Exit 
  // Frame 1
  frames[1].InitWall(0, 0, 4, 128);           // Left Wall 
  frames[1].InitWall(124, 0, 128, 128);       // Right Wall 
  frames[1].InitWall(4, 0, 125, 4);           // Top Wall 
  frames[1].InitWall(4, 124, 125, 128);       // Bottom Wall 
  frames[1].InitWall(30, 70, 90, 74);         // Middle  
  frames[1].InitWall(30, 20, 34, 100);        // Left side 
  frames[1].InitExit(124,59,128,69, 15,60,0); // Right Exit 
  frames[1].InitExit(0,59,4,69, 111,60,2);    // Left Exit Square
  // Frame 2
  frames[2].InitWall(0, 0, 4, 128);           // Left Wall 
  frames[2].InitWall(124, 0, 128, 128);       // Right Wall 
  frames[2].InitWall(4, 0, 125, 4);           // Top Wall 
  frames[2].InitWall(4, 124, 125, 128);       // Bottom Wall 
  frames[2].InitWall(30, 20, 34, 100);        // Left side 
  frames[2].InitWall(30, 96, 70, 100);        // Bottom side 
  frames[2].InitExit(124,59,128,69, 15,60,1); // Right Exit 
  frames[2].InitExit(59,124,69,128, 59,15,3); // Bottom Exit 
  // Frame 3
  frames[3].InitWall(0, 0, 4, 128);           // Left Wall 
  frames[3].InitWall(124, 0, 128, 128);       // Right Wall 
  frames[3].InitWall(4, 0, 125, 4);           // Top Wall 
  frames[3].InitWall(4, 124, 125, 128);       // Bottom Wall 
  frames[3].InitWall(20, 30, 90, 34);         // Top  
  frames[3].InitWall(30, 50, 34, 100);        // Left side 
  frames[3].InitExit(124,59,128,69, 15,60,4); // Right Exit 
  frames[3].InitExit(59,0,69,4, 59,113,2);    // Top Exit 
  // Frame 4
  frames[4].InitWall(0, 0, 4, 128);           // Left Wall 
  frames[4].InitWall(124, 0, 128, 128);       // Right Wall 
  frames[4].InitWall(4, 0, 125, 4);           // Top Wall 
  frames[4].InitWall(4, 124, 125, 128);       // Bottom Wall 
  frames[4].InitWall(20, 30, 90, 34);         // Top  
  frames[4].InitWall(70, 30, 74, 90);         // Middel wall 
  frames[4].InitExit(124,59,128,69, 15,60,5); // Right Exit 
  frames[4].InitExit(0,59,4,69, 111,60,3);    // Left Exit Square
  // Frame 5
  frames[5].InitWall(0, 0, 4, 128);           // Left Wall 
  frames[5].InitWall(124, 0, 128, 128);       // Right Wall 
  frames[5].InitWall(4, 0, 125, 4);           // Top Wall 
  frames[5].InitWall(4, 124, 125, 128);       // Bottom Wall 
  frames[5].InitWall(20, 100, 90, 104);       // Top  
  frames[5].InitWall(86, 30, 90, 104);        // Right Side   
  frames[5].InitExit(59,0,69,4, 59,113,0);    // Top Exit 
  frames[5].InitExit(0,59,4,69, 111,60,4);    // Left Exit Square
  //-----------------------------------------------------------

  // Initilizations for game 
  //-----------------------------------------------------------
  enemys[0].assignSprite(hanSolo);
  UART2_Init();
  UART1_Init();
  b1.Buttons_Init();
  j1.ADC_InitDual(ADC1, 1, 2, ADCVREF_VDDA);
  Sound_Init();
  Sound_Stop();
  DAC5_Init();     
  LED_Init();
  greenOn();
  redOff();
  //-----------------------------------------------------------

  // Display Start Menu 
  //-----------------------------------------------------------
  startMenu(spanish); 
  ST7735_FillScreen(ST7735_BLACK);
  Clock_Delay1ms(2000);
  myDisplay.displayNewScreen(currentFrame);   // Init screen to inital frame
  TimerG12_IntArm(80000000/30,2);             // Start game engine (ISR) @30Hz
  __enable_irq();
  //-----------------------------------------------------------

  //-----------------------------------------------------------
  // Main Thread (LCD):
  //  1) Checks if the enemy is dead, meaning I won
  //  2) Otherwise, If im still alive, display the new state of the game
  //  3) Otherwise, I'm dead, meaning I lost.
  //-----------------------------------------------------------
  while(1){
    while(!myDisplay.DisplayReady){}

      // 1) Won game 
      if(!enemys[0].alive && packets > 20){
        __disable_irq();
        winScreen(*spanish);
        while(1){}
      }

      // 2) Game in progress 
      else if(alive){
        while(!myDisplay.DisplayReady){}
        if(myDisplay.FrameShift){
          myDisplay.frameShift(p1);     
        }
        myDisplay.displayPlayer(p1);
        myDisplay.displayEnemies(currentFrame);
        myDisplay.displayShots(currentFrame);
        myDisplay.DisplayReady = false;
      }

      // 3) Lost Game 
      else if(!alive){
        __disable_irq();
        Clock_Delay1ms(2000);              
        deathScreen(*spanish);
        while(1){}
      }

  }
  //-----------------------------------------------------------
}

