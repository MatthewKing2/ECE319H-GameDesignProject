
#ifndef LCDISPLAY_H_
#define LCDISPLAY_H_
#include <stdint.h>
#include "Frame.h"
#include "Player.h"
#include "../inc/ST7735.h"

class LCD{ 
  public: // Data 
    bool MainMenue;     // Semaphor 
    bool InGame;        // Semaphor
    bool GameOver;      // Semaphor
    bool FrameShift;    // Semaphor
    uint32_t oldFrame;  // Frame player was on before frame shift
    bool DisplayReady;  // Semaphor
    uint16_t PlayerColor;

  public: // Private Functions (only called by this class) (TODO: change to private)
    void displayNewFrame(Frame&, Frame&);   // Refernce to frame
    void displayPlayer(Player&);            // Refernce to Player
    void displayPlayerHud(Player&);         // Draws Players HUD
    //void displayEnemies(&Player);
    //void displayShots(&Player);


  public: // Functions 
    LCD(uint8_t, uint8_t, uint8_t);  // Constructor (takes player RGB)
    void displayMainMenu();
    void displayGameOver();
    void displayGameState();

};
#endif

