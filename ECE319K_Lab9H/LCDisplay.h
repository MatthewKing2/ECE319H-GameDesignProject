
#ifndef LCDISPLAY_H_
#define LCDISPLAY_H_
#include <stdint.h>
#include "Frame.h"
#include "Player.h"

class LCD{ 
  public: // Data 
    bool FrameShift;    // Semaphor for Mailboxes
    uint32_t oldFrame;  // Mailbox 
    uint32_t newFrame;  // Mailbox 
    bool DisplayReady;  // Semaphor
    uint16_t backgroundColor;
    uint16_t wallColor; 
    uint16_t exitColor;

  public: // Functions 
    LCD();                // Constructor 
    void displayNewScreen();   // Initalize screen 
    void frameShift(Player&);         // Erases old frame, shows new
    void displayPlayer(Player&);    
    // void displayPlayerHud(Player&); // Draws Players HUD
    //void displayEnemies(Enemy&);
    //void displayShots(Shots&);

};
#endif

