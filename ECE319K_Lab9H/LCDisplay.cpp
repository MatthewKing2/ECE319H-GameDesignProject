
#include <ti/devices/msp/msp.h>
#include "LCDisplay.h"
#include "../inc/ST7735.h"


// class LCD{ 
//   public: // Data 
//     bool MainMenue;     // Semaphor 
//     bool InGame;        // Semaphor
//     bool GameOver;      // Semaphor
//     bool FrameShift;    // Semaphor
//     uint32_t oldFrame;  // Frame player was on before frame shift
//     bool DisplayReady;  // Semaphor

//   private: // Private Functions (only called by this class)
//     void displayNewFrame(Frame&);   // Refernce to frame
//     void displayPlayer(Player&);    // Refernce to Player
//     //void displayEnemies(&Player);
//     //void displayShots(&Player);


//   public: // Functions 
//     LCD();                      // Constructor 
//     void displayMainMenu();
//     void displayGameOver();
//     void displayGameState();

// };

// Constructor 
LCD::LCD(uint8_t red, uint8_t green, uint8_t blue){
    this->MainMenue = true;
    this->InGame = false;
    this->GameOver = false;
    this->FrameShift = true; // b/c I want to display new map (walls and exits)
    this->oldFrame = 0;      // What will be erased
    this->DisplayReady = false;
    this->PlayerColor = ST7735_Color565(red, green, blue); 
}

// When the player moves frames this must happen:
    // 1) Old frames walls+exits errased 
    // 2) New frames walls+exits displayed
void LCD::displayNewFrame(Frame& newF, Frame& oldF){

    uint16_t backgroundColor = ST7735_Color565(0, 0, 0);
    uint16_t wallColor = ST7735_Color565(255, 255, 255);

    // First N walls are the perimiter, dont need to display those 
    // Erase Old
    uint32_t n = 0;
    for(int i = n; i < oldF.wallsIndex; i ++){
        // TopLeftX, TopLeftY, Wideth, Height
        ST7735_FillRect(oldF.walls[i].TLx, oldF.walls[i].TLy, (oldF.walls[i].BRx-oldF.walls[i].TLx), (oldF.walls[i].BRy-oldF.walls[i].TLy), backgroundColor);
    }
    // Draw new
    for(int i = n; i < newF.wallsIndex; i ++){
        ST7735_FillRect(newF.walls[i].TLx, newF.walls[i].TLy, (newF.walls[i].BRx-newF.walls[i].TLx), (newF.walls[i].BRy-newF.walls[i].TLy), wallColor);
    }

}

// All you have to do is fill rectange on the new player X and Y
// Player automatically erases itself (b/c of background buffer)
    // TODO fix this function to draw bit map
    // Can also get rid of player color then lmao
void LCD::displayPlayer(Player& p1){
    ST7735_FillRect(p1.x_position(), p1.y_position(), 8, 8, this->PlayerColor);
}
