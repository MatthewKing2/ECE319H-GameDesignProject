
#ifndef LCDISPLAY_H_
#define LCDISPLAY_H_
#include <stdint.h>
#include "Frame.h"
#include "Player.h"
#include "Enemy.h"

class LCD{ 
  public: // Data 
    bool FrameShift;    // Semaphor for Mailboxes
    uint32_t oldFrame;  // Mailbox 
    uint32_t newFrame;  // Mailbox 
    bool DisplayReady;  // Semaphor
    uint16_t backgroundColor;
    uint16_t wallColor; 
    uint16_t exitColor;
    uint16_t shotColor;

  public: // Functions 
    LCD();                                    // Constructor 
    void displayNewScreen(uint32_t currFrame);// Initalize screen 
    void frameShift(Player&);                 // Erases old frame, shows new
    void displayPlayer(Player&);              // Displays Player
    // void displayPlayerHud(Player&);        // Draws Players HUD
    void displayEnemies(uint32_t currFrame);  // Displays Enemey in this frame;
    void clearSpecificEnemy(Enemy& e1);          // Used when an ememy leaves my frame (logic in main)
    void displayShots(uint32_t currentFrame);
    void clearShots(uint32_t currentFrame);
      // Might have a small issue if someone shoots right when they frame shift 
      // nvm i shoot after i move

};
#endif

