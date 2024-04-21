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
#include "Sound.h"
#include "images/images.h"
// My custom code 
#include "Player.h"
#include "MyJoystick.h"
#include "../inc/ADC.h"   // Using these ADC functions to set up and sample
#include "Frame.h"
#include "LCDisplay.h"
extern "C" void __disable_irq(void);
extern "C" void __enable_irq(void);
extern "C" void TIMG12_IRQHandler(void);



// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
void PLL_Init(void){ // set phase lock loop (PLL)
  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

uint32_t M=1;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}




SlidePot Sensor(1500,0); // copy calibration from Lab 7

// games  engine runs at 30Hz
void TIMG12_IRQHandler(void){uint32_t pos,msg;
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
// game engine goes here
    // 1) sample slide pot
    // 2) read input switches
    // 3) move sprites
    // 4) start sounds
    // 5) set semaphore
    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    GPIOB->DOUTTGL31_0 = GREEN; // toggle PB27 (minimally intrusive debugging)
  }
}
uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}







typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=English;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};
// use main1 to observe special characters
int main1(void){ // main1
    char l;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(0x0000);            // set screen to black
  for(int myPhrase=0; myPhrase<= 2; myPhrase++){
    for(int myL=0; myL<= 3; myL++){
         ST7735_OutString((char *)Phrases[LANGUAGE][myL]);
      ST7735_OutChar(' ');
         ST7735_OutString((char *)Phrases[myPhrase][myL]);
      ST7735_OutChar(13);
    }
  }
  Clock_Delay1ms(3000);
  ST7735_FillScreen(0x0000);       // set screen to black
  l = 128;
  while(1){
    Clock_Delay1ms(2000);
    for(int j=0; j < 3; j++){
      for(int i=0;i<16;i++){
        ST7735_SetCursor(7*j+0,i);
        ST7735_OutUDec(l);
        ST7735_OutChar(' ');
        ST7735_OutChar(' ');
        ST7735_SetCursor(7*j+4,i);
        ST7735_OutChar(l);
        l++;
      }
    }
  }
}


// Test Joystick Main
int mainJoystick(void){

  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(ST7735_BLACK);
  while(1){
  
    ST7735_FillScreen(0x0000);   // set screen to black
    ST7735_SetRotation(3);

    uint16_t white = ST7735_Color565(255, 255, 255);
    uint16_t red = ST7735_Color565(255, 0, 0);
    uint16_t green = ST7735_Color565(0, 255, 0);
    uint16_t blue = ST7735_Color565(0, 0, 255);

    // Player p1(60, 60, false);
    // Player p2(60, 60, false);
    // //Player p2(160-8, 128-8, 0, false);
    // Player p3(160-8, 0, false);  
    // Player p4(0, 128-8, false);

    // Set up joystick
    Joystick j1;
    j1.ADC_InitDual(ADC1, 1, 2, ADCVREF_VDDA);


    for(int j = 1; j < 200; j ++){
      // ST7735_FillRect(p1.x_position(), p1.y_position(), 8, 8, white);
      // ST7735_FillRect(p2.x_position(), p2.y_position(), 8, 8, red);   // Proves that most recent write wins on the LCD (minor flicker good enough for now)
    //   ST7735_FillRect(p3.x_position(), p3.y_position(), 8, 8, green);
    //   ST7735_FillRect(p4.x_position(), p4.y_position(), 8, 8, blue);

      // Read input 
      uint32_t x = 0;
      uint32_t y = 0;
      j1.ADC_InDual(ADC1, &x, &y);
      // Old stuff would have to add a frame to see if its working
      // p1.moveLinear(x, y);         
      // p2.moveLinear(x, y);


      Clock_Delay1ms(33);              
    }

    } // end of while(1)
}



// Test and see if we can:
  // 1) Create Frames (and walls)
  // 2) Display Frames  
int mainTestLCD1(void){

  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(ST7735_BLACK);
  while(1){
  
    ST7735_FillScreen(0x0000);   // set screen to black
    ST7735_SetRotation(3);
    
    // Frame's Constructor
    // Frame(uint32_t frameNumber);        // Constructor 
    // Frame f0(0);
    // Frame f1(1);
    // f1.InitWall(10, 10, 12, 100); // Long Vertical line
    //LCD display();       // Red player
    //display.displayNewFrame(f1, f0);
    
    Clock_Delay1ms(2000);              

  }
}


// Test Frames and Player Colision 
//int mainTestLCD2(){
//
//  __disable_irq();
//  PLL_Init(); // set bus speed
//  LaunchPad_Init();
//  ST7735_InitPrintf();
//  ST7735_FillScreen(ST7735_BLACK);
//  while(1){
//
//    uint16_t red = ST7735_Color565(255, 0, 0);
//    ST7735_FillScreen(0x0000);   // set screen to black
//    ST7735_SetRotation(3);
//
//    // set up joystick
//    Joystick j1;
//    j1.ADC_InitDual(ADC1, 1, 2, ADCVREF_VDDA);
//
//    // Set up Player
//    Player p1(60, 60, false);
//
//    // Frame's Constructor
//    // Frame(uint32_t frameNumber);        // Constructor
//    Frame f0;
//    Frame f1;
//    f1.InitWall(0, 0, 2, 128);      // Left Wall
//    f1.InitWall(125, 0, 128, 128);  // Right Wall
//    f1.InitWall(3, 0, 125, 2);      // Top Wall
//    f1.InitWall(3, 126, 125, 128);  // Bottom Wall
//    f1.InitExit(10,10,20,20, 30,30,0);       // Random Square
//      // Sends player to 30,30 on F0
//      // Not implimented yet
//
//    // LCD display(255, 0, 0);       // Red player
//    // display.displayNewFrame(f1, f0);
//
//    for(int j = 1; j < 200; j ++){
//      ST7735_FillRect(p1.x_position(), p1.y_position(), 8, 8, red);
//
//      // Read input
//      uint32_t x = 0;
//      uint32_t y = 0;
//      j1.ADC_InDual(ADC1, &x, &y);
//      p1.moveLinear(x, y, 0);
//
//      Clock_Delay1ms(33);
//    }
//
//  }
//}


// Test Frames Telaporation 
Frame frames[9];          // Global Array of Frames 

int main(){
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_SetRotation(3);
  uint16_t red = ST7735_Color565(255, 0, 0);

  // Frame 0
  frames[0].InitWall(0, 0, 2, 128);      // Left Wall 
  frames[0].InitWall(125, 0, 128, 128);  // Right Wall 
  frames[0].InitWall(3, 0, 125, 2);      // Top Wall 
  frames[0].InitWall(3, 126, 125, 128);  // Bottom Wall 
  frames[0].InitExit(0,59,4,69, 111,60,1); // Random Square
  // Frame 1
  frames[1].InitWall(0, 0, 2, 128);      // Left Wall 
  frames[1].InitWall(126, 0, 128, 128);  // Right Wall 
  frames[1].InitWall(3, 0, 125, 2);      // Top Wall 
  frames[1].InitWall(3, 126, 125, 128);  // Bottom Wall 
  frames[1].InitExit(124,59,128,69, 15,60,0); // Random Square

  // Set up for Game (constant stuff)
  //##################################################  
  //##################################################  
  // Player 
  // Joystick
  // LCD 
  // Current Frame
  Joystick j1;
    j1.ADC_InitDual(ADC1, 1, 2, ADCVREF_VDDA);
  Player p1(60, 60, redPlayer8x8, false);
  LCD myDisplay;                  // Red player
  myDisplay.displayNewScreen();   // Init screen 
  uint32_t currentFrame = 0;
  //##################################################  
  //##################################################  

  // Write this like G12 + Main
  while(1){
  // Timer G12: 
  //##################################################  
    // Alive
    // Exit 
      bool exit = false;
      uint32_t newFrame;
      exit = p1.touchingExit(currentFrame, &newFrame);
        // Outputs: exit T/F, and newFrame Index
      if(exit){
        myDisplay.FrameShift = true;
        myDisplay.oldFrame = currentFrame;
        myDisplay.newFrame = newFrame;
        currentFrame = newFrame;
      }
    // Read Joystick 
      uint32_t x = 0;
      uint32_t y = 0;
      j1.ADC_InDual(ADC1, &x, &y);
    // Move as much as possible (on their current frame)
      p1.moveLinear(x, y, currentFrame);         
    // Shoot 
    // Send my Info
    // Spin until i get everyones data
      // Update Enemies
      // Update Shots
    // LCD Read
    myDisplay.DisplayReady = true;
  //##################################################  

  // Main Thread (LCD):
  //##################################################  
    // Spin on ready Semaphor
    while(!myDisplay.DisplayReady){}
    // Frameshift? 
    if(myDisplay.FrameShift){
      myDisplay.frameShift(p1);     // Needs to Erase the player & enemies 
    }
    // Display Player
    myDisplay.displayPlayer(p1);
    // Display Enemies
    // Display Shots
    // Display Hud
    // Clear Semaphor
    myDisplay.DisplayReady = false;
  //##################################################  

  // Just for testing:
  Clock_Delay1ms(200);              
   
  }
}


// use main2 to observe graphics
    int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  // ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  // ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  // ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  // ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  // ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom
  // ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  // ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  // ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  // ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  // ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);

  // for(uint32_t t=500;t>0;t=t-5){
  //   SmallFont_OutVertical(t,104,6); // top left
  //   Clock_Delay1ms(50);              // delay 50 msec
  // }
  // ST7735_FillScreen(0x0000);   // set screen to black
  // ST7735_SetCursor(1, 1);
  // ST7735_OutString((char *)"GAME OVER");
  // ST7735_SetCursor(1, 2);
  // ST7735_OutString((char *)"Nice try,");
  // ST7735_SetCursor(1, 3);
  // ST7735_OutString((char *)"Earthling!");
  // ST7735_SetCursor(2, 4);
  // ST7735_OutUDec(1234);
  while(1){
      // Clock_Delay1ms(1000);
  
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetRotation(3);
  // MaxX = 160-(Size+1)
  // MaxY = 128-(Size+1)
  // Now trying to do stuff with the player 
  uint16_t white = ST7735_Color565(255, 255, 255);
  uint16_t red = ST7735_Color565(255, 0, 0);
  uint16_t green = ST7735_Color565(0, 255, 0);
  uint16_t blue = ST7735_Color565(0, 0, 255);

  // Player p1(60, 60, 0, false);
  // Player p2(160-8, 128-8, 0, false);
  // Player p3(160-8, 0, 0, false);  
  // Player p4(0, 128-8, 0, false);
   // ST7735_FillScreen(0x0000);   // set screen to black


      // Set up joystick
      Joystick j1;
      j1.ADC_InitDual(ADC1, 1, 2, ADCVREF_VDDA);


      // char msg[10] = {'H','e','l', 'l', 'o', ' ', 'B', 'o', 'b', '\0'};
      // ST7735_SetCursor(1, 1);
      // ST7735_OutString(msg);

      for(int j = 1; j < 200; j ++){
        // ST7735_FillRect(p1.x_position(), p1.y_position(), 8, 8, white);
      //   ST7735_FillRect(p2.x_position(), p2.y_position(), 8, 8, red);
      //   ST7735_FillRect(p3.x_position(), p3.y_position(), 8, 8, green);
      //   ST7735_FillRect(p4.x_position(), p4.y_position(), 8, 8, blue);

        // // Right one, Down two
        // p1.move(1, -2);         
        // // Left one, Up two
        // p2.move(-1, 2);         
        // // Left two, Down one 
        // p3.move(-2, -1);        
        // // Right two, up one
        // p4.move(2, 1);         

        // Read input 
        uint32_t x = 0;
        uint32_t y = 0;
        j1.ADC_InDual(ADC1, &x, &y);
        // p1.moveExpo(x, y);         

        // // Left one, Up two
        // p2.move(1024, 4096);         
        // // Left two, Down one 
        // p3.move(0, 1024);        
        // // Right two, up one
        // p4.move(4096, 3072);         

        Clock_Delay1ms(33);              
      }

    } // end of while(1)

  }






// use main3 to test switches and LEDs (and ADC)
int main3(void){ // main3
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  ST7735_InitPrintf();
  LED_Init(); // initialize LED
  while(1){
    // write code to test switches and LEDs
  
      Clock_Delay1ms(1000);
      ST7735_FillScreen(0x0000);   // set screen to black
      ST7735_SetRotation(3);

      // Set up joystick
      // Joystick j1;
      // j1.Joystick_Init();

      // // Read input 
      // uint32_t x; 
      // uint32_t y; 
      // j1.Joystick_In(&x, &y);

      // Set up joystick
      Joystick j1;
      j1.ADC_InitDual(ADC1, 1, 2, ADCVREF_VDDA);

      // Read input 
      uint32_t x = 0;
      uint32_t y = 0;
      j1.ADC_InDual(ADC1, &x, &y);

      // Print to LCD
      ST7735_SetCursor(10,10);
      printf("%d", x);

      ST7735_SetCursor(80,80);
      printf("%d", y);



  }
}








// use main4 to test sound outputs
int main4(void){ uint32_t last=0,now;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  Switch_Init(); // initialize switches
  LED_Init(); // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(ADC0,6,0); // ADC1 channel 6 is PB20, TExaS scope
  __enable_irq();
  while(1){
    now = Switch_In(); // one of your buttons
    if((last == 0)&&(now == 1)){
      Sound_Shoot(); // call one of your sounds
    }
    if((last == 0)&&(now == 2)){
      Sound_Killed(); // call one of your sounds
    }
    if((last == 0)&&(now == 4)){
      Sound_Explosion(); // call one of your sounds
    }
    if((last == 0)&&(now == 8)){
      Sound_Fastinvader1(); // call one of your sounds
    }
    // modify this to test all your sounds
  }
}







// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main5(void){ // final main
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  Sensor.Init(); // PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(80000000/30,2);
  // initialize all data structures
  __enable_irq();

  while(1){
    // wait for semaphore
       // clear semaphore
       // update ST7735R
    // check for end game or level switch
  }
}
