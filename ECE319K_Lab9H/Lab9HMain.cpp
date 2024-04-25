// Lab9HMain.cpp
// Runs on MSPM0G3507
// Lab 9 ECE319H
// Your name
// Last Modified: 1/1/2024

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
// My custom code 
#include "Player.h"
#include "MyJoystick.h"
#include "../inc/ADC.h"   
#include "Frame.h"
#include "LCDisplay.h"
#include "LaserShot.h"
#include "Enemy.h"
#include "../inc/FIFO2.cpp"   // Make file is not finding the code, therefore .cpp instead of .h 
#include "GameUart.h"
#include "MyButtons.h"
#include "MenuScreens.h"      // Simons Files 
#include "MySounds.h"         // Simons Files
extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);



// Microcontroller Clock
void PLL_Init(void){ // set phase lock loop (PLL)
  Clock_Init80MHz(0);   // run this line for 80MHz
}

// Pin setupt for Texas Scope 
uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}





// Set up for Game (Global Vars)
//##################################################  
//##################################################  
// Set up Map and Enemies
Frame frames[9];          // Global Array of Frames 
Shot shots[4];            // Global Array of Shots
Enemy enemys[3];          // Global Array of Enemys
// Set up Player
Player p1(60, 60, redPlayer8x8, false);
uint32_t currentFrame = 0;    // Change this to make player spawn in differnt frame 
bool alive = true;
uint32_t health = 30;
// Sound vars 
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
//##################################################  
//##################################################  





// Timer G12 Game Engine: 
//##################################################  
void TIMG12_IRQHandler(void){
    // Alive
    if(p1.touchingLaser(currentFrame)){
      if(health == 1){
        alive = false;
        greenOff();
        redOn();
      }else{
        health --;
      }
    } 
    // Clear Shots (MUST be after alive check)
      myDisplay.clearShots(currentFrame);
    // Exit 
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
    // Read Joystick 
      uint32_t x = 0;
      uint32_t y = 0;
      j1.ADC_InDual(ADC1, &x, &y);
    // Move as much as possible (on their current frame)
      p1.moveLinear(x, y, currentFrame);         
    // Shoot 
      // Read buttons 
      uint32_t up;
      uint32_t down;
      uint32_t left;
      uint32_t right;
      uint32_t dash;
      b1.Buttons_In(&up, &down, &left, &right, &dash);
      bool shot = false;
      uint32_t shotDirection = 0; // Arbirary default 
      if(down && !dash && !left && !right){
        shots[0].generate(p1.x,p1.y, 1,currentFrame); // Shot upwards @ player x, y
        shot = true;
        shotDirection = 1;
      }
      else if(left && !down && !dash && !right){
        shots[0].generate(p1.x,p1.y, 2,currentFrame); // Shot upwards @ player x, y
        shot = true;
        shotDirection = 2;
      }
      else if(right && !dash && !down && !left){
        shots[0].generate(p1.x,p1.y, 3,currentFrame); // Shot upwards @ player x, y
        shot = true;
        shotDirection = 3;
      }
      else if(dash && !down && !right && !left){
        shots[0].generate(p1.x,p1.y, 0,currentFrame); // Shot upwards @ player x, y
        shot = true;
        shotDirection = 0;
      }
      // Play the shooting sound when player shoots
      if(shot){
        sound = 2;
        Sound_Start();
      }
    // Send my Info
    UART1_Transmit(1, currentFrame, alive, false, shot, shotDirection, p1.x, p1.y);
    // Read data out of SW Fifo 
      // #####################################
      bool newData = false;
      char data1;
      char data2;
      char data3;
      char data4;
      //UART2_InData(&data1, &data2, &data3, &data4);
      data1 = UART2_InChar();
      uint32_t stopper = 0;
      while((data1 != ((1<<7)|1)) && stopper < 4){
        //UART2_InData(&data1, &data2, &data3, &data4);
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
        // Toggle Pin (minimally Intrusive)
        GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
      }
      // #####################################
      // Update Enemies
      enemys[0].updatePosition(r1.x, r1.y, r1.frame, r1.alive);
      // Update Shots
      if(r1.shot){
        shots[1].generate(enemys[0].x, enemys[0].y, r1.shotDirection, enemys[0].frame);
      }
    // LCD Read
    myDisplay.DisplayReady = true;
  //##################################################  

}





// Main Thread (Inits and LCD)
//##################################################  
int main(){
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_SetRotation(3);
  uint16_t red = ST7735_Color565(255, 0, 0);

  // Frame 0
  frames[0].InitWall(0, 0, 4, 128);      // Left Wall 
  frames[0].InitWall(124, 0, 128, 128);  // Right Wall 
  frames[0].InitWall(4, 0, 125, 4);      // Top Wall 
  frames[0].InitWall(4, 124, 125, 128);  // Bottom Wall 
  frames[0].InitWall(20, 20, 90, 24);     // Top  
  frames[0].InitWall(90, 70, 94, 100);     // Right Side   
  frames[0].InitExit(0,59,4,69, 111,60,1); // Left Exit 
  frames[0].InitExit(59,124,69,128, 59,15,5); // Bottom Exit 
  // Frame 1
  frames[1].InitWall(0, 0, 4, 128);      // Left Wall 
  frames[1].InitWall(124, 0, 128, 128);  // Right Wall 
  frames[1].InitWall(4, 0, 125, 4);      // Top Wall 
  frames[1].InitWall(4, 124, 125, 128);  // Bottom Wall 
  frames[1].InitWall(30, 70, 90, 74);      // Middle  
  frames[1].InitWall(30, 20, 34, 100);  // Left side 
  frames[1].InitExit(124,59,128,69, 15,60,0); // Right Exit 
  frames[1].InitExit(0,59,4,69, 111,60,2); // Left Exit Square
  // Frame 2
  frames[2].InitWall(0, 0, 4, 128);      // Left Wall 
  frames[2].InitWall(124, 0, 128, 128);  // Right Wall 
  frames[2].InitWall(4, 0, 125, 4);      // Top Wall 
  frames[2].InitWall(4, 124, 125, 128);  // Bottom Wall 
  frames[2].InitWall(30, 20, 34, 100);  // Left side 
  frames[2].InitWall(30, 96, 70, 100);  // Bottom side 
  frames[2].InitExit(124,59,128,69, 15,60,1); // Right Exit 
  frames[2].InitExit(59,124,69,128, 59,15,3); // Bottom Exit 
  // Frame 3
  frames[3].InitWall(0, 0, 4, 128);      // Left Wall 
  frames[3].InitWall(124, 0, 128, 128);  // Right Wall 
  frames[3].InitWall(4, 0, 125, 4);      // Top Wall 
  frames[3].InitWall(4, 124, 125, 128);  // Bottom Wall 
  frames[3].InitWall(20, 30, 90, 34);     // Top  
  frames[3].InitWall(30, 50, 34, 100);  // Left side 
  frames[3].InitExit(124,59,128,69, 15,60,4); // Right Exit 
  frames[3].InitExit(59,0,69,4, 59,113,2); // Top Exit 
  // Frame 4
  frames[4].InitWall(0, 0, 4, 128);      // Left Wall 
  frames[4].InitWall(124, 0, 128, 128);  // Right Wall 
  frames[4].InitWall(4, 0, 125, 4);      // Top Wall 
  frames[4].InitWall(4, 124, 125, 128);  // Bottom Wall 
  frames[4].InitWall(20, 30, 90, 34);     // Top  
  frames[4].InitWall(70, 30, 74, 90);  // Middel wall 
  frames[4].InitExit(124,59,128,69, 15,60,5); // Right Exit 
  frames[4].InitExit(0,59,4,69, 111,60,3); // Left Exit Square
  // Frame 5
  frames[5].InitWall(0, 0, 4, 128);      // Left Wall 
  frames[5].InitWall(124, 0, 128, 128);  // Right Wall 
  frames[5].InitWall(4, 0, 125, 4);      // Top Wall 
  frames[5].InitWall(4, 124, 125, 128);  // Bottom Wall 
  frames[5].InitWall(20, 100, 90, 104);     // Top  
  frames[5].InitWall(86, 30, 90, 104);     // Right Side   
  frames[5].InitExit(59,0,69,4, 59,113,0); // Top Exit 
  frames[5].InitExit(0,59,4,69, 111,60,4); // Left Exit Square

  // Initilizations for game 
  //##################################################  
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
  //##################################################  

  // Display Start Menu 
  startMenu(spanish); 
  // Transition to game 
  ST7735_FillScreen(ST7735_BLACK);
  Clock_Delay1ms(2000);
  myDisplay.displayNewScreen(currentFrame);   // Init screen to inital frame
  TimerG12_IntArm(80000000/30,2); // Start game engine @30Hz
  __enable_irq();

// Main Thread (LCD):
//##################################################  
  while(1){
    // Spin on ready Semaphor
    while(!myDisplay.DisplayReady){}
      // If i won, say I won
      if(!enemys[0].alive && packets > 20){
          // Dispay you won
        winScreen(*spanish);
        while(1){}
      }
      // Otherwise, if im alive, dispay the new game state
      else if(alive){
        while(!myDisplay.DisplayReady){}
        // Frameshift? 
        if(myDisplay.FrameShift){
          myDisplay.frameShift(p1);     // Needs to Erase the player & enemies 
        }
        // Display Player
        myDisplay.displayPlayer(p1);
        // Display Enemies
        myDisplay.displayEnemies(currentFrame);
        // Display Shots
        myDisplay.displayShots(currentFrame);
        // Display Hud
        // Clear Semaphor
        myDisplay.DisplayReady = false;
      }
      // Otherwise if im dead, say i died 
      else if(!alive){
        Clock_Delay1ms(2000);              
        deathScreen(*spanish);
        while(1){}
      }
  }
//##################################################  
}







// Old Game Set up (Game Engine was in main thread)

// Frame frames[9];          // Global Array of Frames 
// Shot shots[4];            // Global Array of Shots
// Enemy enemys[3];          // Global Array of Enemys


// int oldWalls(){
//   // Frame 0
//   frames[0].InitWall(0, 0, 2, 128);      // Left Wall 
//   frames[0].InitWall(125, 0, 128, 128);  // Right Wall 
//   frames[0].InitWall(3, 0, 125, 2);      // Top Wall 
//   frames[0].InitWall(3, 126, 125, 128);  // Bottom Wall 
//   frames[0].InitExit(0,59,4,69, 111,60,1); // Random Square
//   // Frame 1
//   frames[1].InitWall(0, 0, 2, 128);      // Left Wall 
//   frames[1].InitWall(126, 0, 128, 128);  // Right Wall 
//   frames[1].InitWall(3, 0, 125, 2);      // Top Wall 
//   frames[1].InitWall(3, 126, 125, 128);  // Bottom Wall 
//   frames[1].InitExit(124,59,128,69, 15,60,0); // Random Square
//   return 0;
// }

// int mainTestingGame(){
//   __disable_irq();
//   PLL_Init(); // set bus speed
//   LaunchPad_Init();
//   ST7735_InitPrintf();
//   ST7735_FillScreen(ST7735_BLACK);
//   ST7735_SetRotation(3);
//   uint16_t red = ST7735_Color565(255, 0, 0);

//   // Frame 0
//   frames[0].InitWall(0, 0, 4, 128);      // Left Wall 
//   frames[0].InitWall(124, 0, 128, 128);  // Right Wall 
//   frames[0].InitWall(4, 0, 125, 4);      // Top Wall 
//   frames[0].InitWall(4, 124, 125, 128);  // Bottom Wall 
//   frames[0].InitWall(20, 20, 90, 24);     // Top  
//   frames[0].InitWall(90, 70, 94, 100);     // Right Side   
//   frames[0].InitExit(0,59,4,69, 111,60,1); // Left Exit 
//   frames[0].InitExit(59,124,69,128, 59,15,5); // Bottom Exit 
//   // Frame 1
//   frames[1].InitWall(0, 0, 4, 128);      // Left Wall 
//   frames[1].InitWall(124, 0, 128, 128);  // Right Wall 
//   frames[1].InitWall(4, 0, 125, 4);      // Top Wall 
//   frames[1].InitWall(4, 124, 125, 128);  // Bottom Wall 
//   frames[1].InitWall(30, 70, 90, 74);      // Middle  
//   frames[1].InitWall(30, 20, 34, 100);  // Left side 
//   frames[1].InitExit(124,59,128,69, 15,60,0); // Right Exit 
//   frames[1].InitExit(0,59,4,69, 111,60,2); // Left Exit Square
//   // Frame 2
//   frames[2].InitWall(0, 0, 4, 128);      // Left Wall 
//   frames[2].InitWall(124, 0, 128, 128);  // Right Wall 
//   frames[2].InitWall(4, 0, 125, 4);      // Top Wall 
//   frames[2].InitWall(4, 124, 125, 128);  // Bottom Wall 
//   frames[2].InitWall(30, 20, 34, 100);  // Left side 
//   frames[2].InitWall(30, 96, 70, 100);  // Bottom side 
//   frames[2].InitExit(124,59,128,69, 15,60,1); // Right Exit 
//   frames[2].InitExit(59,124,69,128, 59,15,3); // Bottom Exit 
//   // Frame 3
//   frames[3].InitWall(0, 0, 4, 128);      // Left Wall 
//   frames[3].InitWall(124, 0, 128, 128);  // Right Wall 
//   frames[3].InitWall(4, 0, 125, 4);      // Top Wall 
//   frames[3].InitWall(4, 124, 125, 128);  // Bottom Wall 
//   frames[3].InitWall(20, 30, 90, 34);     // Top  
//   frames[3].InitWall(30, 50, 34, 100);  // Left side 
//   frames[3].InitExit(124,59,128,69, 15,60,4); // Right Exit 
//   frames[3].InitExit(59,0,69,4, 59,113,2); // Top Exit 
//   // Frame 4
//   frames[4].InitWall(0, 0, 4, 128);      // Left Wall 
//   frames[4].InitWall(124, 0, 128, 128);  // Right Wall 
//   frames[4].InitWall(4, 0, 125, 4);      // Top Wall 
//   frames[4].InitWall(4, 124, 125, 128);  // Bottom Wall 
//   frames[4].InitWall(20, 30, 90, 34);     // Top  
//   frames[4].InitWall(70, 30, 74, 90);  // Middel wall 
//   frames[4].InitExit(124,59,128,69, 15,60,5); // Right Exit 
//   frames[4].InitExit(0,59,4,69, 111,60,3); // Left Exit Square
//   // Frame 5
//   frames[5].InitWall(0, 0, 4, 128);      // Left Wall 
//   frames[5].InitWall(124, 0, 128, 128);  // Right Wall 
//   frames[5].InitWall(4, 0, 125, 4);      // Top Wall 
//   frames[5].InitWall(4, 124, 125, 128);  // Bottom Wall 
//   frames[5].InitWall(20, 100, 90, 104);     // Top  
//   frames[5].InitWall(86, 30, 90, 104);     // Right Side   
//   frames[5].InitExit(59,0,69,4, 59,113,0); // Top Exit 
//   frames[5].InitExit(0,59,4,69, 111,60,4); // Left Exit Square

//   // Set up for Game (constant stuff)
//   //##################################################  
//   //##################################################  
//   UART2_Init();
//   UART1_Init();
//   // Buttons
//   Buttons b1;
//     b1.Buttons_Init();
//   // Joystick
//   Joystick j1;
//     j1.ADC_InitDual(ADC1, 1, 2, ADCVREF_VDDA);
//   // Player 
//   Player p1(60, 60, redPlayer8x8, false);
//   // Receiver (gets uart data to generate enemies)
//   uint32_t packets = 0;
//   Receiver r1;
//   // Current Frame
//   uint32_t currentFrame = 3;
//   // LCD 
//   LCD myDisplay;                  
//   // Sound 
//   Sound_Init();
//   Sound_Stop();
//   DAC5_Init();     
//   LED_Init();
//   bool s = false;
//   bool* spanish = &s;
//   //##################################################  
//   //##################################################  

//   // Start of Menue
//   startMenu(spanish); // funtion returns when player hits ready
//   ST7735_FillScreen(ST7735_BLACK);
//   Clock_Delay1ms(2000);
//   __enable_irq();

//   // Write this like G12 + Main
//   bool alive = true;
//   uint32_t health = 30;
//   myDisplay.displayNewScreen(currentFrame);   // Init screen to inital frame
//   greenOn();
//   redOff();
//   while(1){
//   // Timer G12: 
//   //##################################################  
//     // Alive
//     if(p1.touchingLaser(currentFrame)){
//       if(health == 1){
//         alive = false;
//         greenOff();
//         redOn();
//       }else{
//         health --;
//       }
//     } 
//     // Clear Shots (MUST be after alive check)
//       myDisplay.clearShots(currentFrame);
//     // Exit 
//       bool exit = false;
//       uint32_t newFrame;
//       exit = p1.touchingExit(currentFrame, &newFrame);
//       if(exit){
//         myDisplay.FrameShift = true;
//         myDisplay.oldFrame = currentFrame;
//         myDisplay.newFrame = newFrame;
//         currentFrame = newFrame;
//         sound = 0;
//         Sound_Start();
//       }
//     // Read Joystick 
//       uint32_t x = 0;
//       uint32_t y = 0;
//       j1.ADC_InDual(ADC1, &x, &y);
//     // Move as much as possible (on their current frame)
//       p1.moveLinear(x, y, currentFrame);         
//     // Shoot 
//       // Read buttons 
//       uint32_t up;
//       uint32_t down;
//       uint32_t left;
//       uint32_t right;
//       uint32_t dash;
//       b1.Buttons_In(&up, &down, &left, &right, &dash);
//       bool shot = false;
//       uint32_t shotDirection = 0; // Arbirary default 
//       if(down && !dash && !left && !right){
//         shots[0].generate(p1.x,p1.y, 1,currentFrame); // Shot upwards @ player x, y
//         shot = true;
//         shotDirection = 1;
//       }
//       else if(left && !down && !dash && !right){
//         shots[0].generate(p1.x,p1.y, 2,currentFrame); // Shot upwards @ player x, y
//         shot = true;
//         shotDirection = 2;
//       }
//       else if(right && !dash && !down && !left){
//         shots[0].generate(p1.x,p1.y, 3,currentFrame); // Shot upwards @ player x, y
//         shot = true;
//         shotDirection = 3;
//       }
//       else if(dash && !down && !right && !left){
//         shots[0].generate(p1.x,p1.y, 0,currentFrame); // Shot upwards @ player x, y
//         shot = true;
//         shotDirection = 0;
//       }
//       // Play the shooting sound when player shoots
//       if(shot){
//         sound = 2;
//         Sound_Start();
//       }
//     // Send my Info
//     UART1_Transmit(1, currentFrame, alive, false, shot, shotDirection, p1.x, p1.y);
//     // Read data out of SW Fifo 
//       // #####################################
//       bool newData = false;
//       char data1;
//       char data2;
//       char data3;
//       char data4;
//       //UART2_InData(&data1, &data2, &data3, &data4);
//       data1 = UART2_InChar();
//       uint32_t stopper = 0;
//       while((data1 != ((1<<7)|1)) && stopper < 4){
//         //UART2_InData(&data1, &data2, &data3, &data4);
//         data1 = UART2_InChar();
//         stopper ++;
//       }
//       if(stopper != 4){
//         newData = true;  
//         data2 = UART2_InChar();
//         data3 = UART2_InChar();
//         data4 = UART2_InChar();
//         packets ++;
//         r1.receiverTranslate(data1, data2, data3, data4);
//         // Toggle Pin (minimally Intrusive)
//         GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
//       }
//       // #####################################
//       // Update Enemies
//       enemys[0].updatePosition(r1.x, r1.y, r1.frame, r1.alive);
//       // Update Shots
//       if(r1.shot){
//         shots[1].generate(enemys[0].x, enemys[0].y, r1.shotDirection, enemys[0].frame);
//       }
//     // LCD Read
//     myDisplay.DisplayReady = true;
//   //##################################################  

//   // Main Thread (LCD):
//   //##################################################  
//     // Spin on ready Semaphor
//     // If i won, say I won
//     if(!enemys[0].alive && packets > 20){
//         // Dispay you won
//       winScreen(*spanish);
//       while(1){}
//     }
//     // Otherwise, if im alive, dispay the new game state
//     else if(alive){
//       while(!myDisplay.DisplayReady){}
//       // Frameshift? 
//       if(myDisplay.FrameShift){
//         myDisplay.frameShift(p1);     // Needs to Erase the player & enemies 
//       }
//       // Display Player
//       myDisplay.displayPlayer(p1);
//       // Display Enemies
//       myDisplay.displayEnemies(currentFrame);
//       // Display Shots
//       myDisplay.displayShots(currentFrame);
//       // Display Hud
//       // Clear Semaphor
//       myDisplay.DisplayReady = false;
//     }
//     // Otherwise if im dead, say i died 
//     else if(!alive){
//       Clock_Delay1ms(2000);              
//       deathScreen(*spanish);
//       while(1){}
//       //endScreen(*spanish);  // this goes forever
//     }
//   //##################################################  

//   // Just for testing:
//   Clock_Delay1ms(33);              
//   }
// }


