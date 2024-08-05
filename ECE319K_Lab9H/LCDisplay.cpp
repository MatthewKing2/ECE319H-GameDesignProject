
//------------------------------------------------------------------------------
// File: LCDisplay.cpp
// Author: Matthew King
// Description: Implementation of the LCD class methods for managing and 
//              rendering game elements on the LCD display. Including
//              the game map, player, shots, and enemies. 
// Dependencies: LCDisplay.h, ST7735.h, LaserShot.h, Enemy.h
//------------------------------------------------------------------------------ 

#include <ti/devices/msp/msp.h>
#include "LCDisplay.h"
#include "../inc/ST7735.h"
#include "LaserShot.h"
#include "Enemy.h"
extern Frame frames[];
extern Shot shots[];
extern Enemy enemys[];          

//------------------------------------------------------------------------------  
// Constructor: LCD()
// Description: Initializes the LCD object with default values. The colors are
//              set using the ST7735 color conversion function for consistency 
//              with the display hardware.
//------------------------------------------------------------------------------  
LCD::LCD(){
    this->FrameShift = false;       // Semphaor 
    this->oldFrame = 0;             // Mailbox 
    this->newFrame = 0;             // Mailbox
    this->DisplayReady = false;     // Not ready yet
    this->backgroundColor = ST7735_Color565(0, 0, 0);       // Black background
    this->wallColor = ST7735_Color565(120, 120, 120);       // Light blue grey for walls
    this->exitColor = ST7735_Color565(60, 60, 60);          // Dark pure grey for exits
    this->shotColor = ST7735_Color565(255, 0, 0);           // Pure red for shots

}

//------------------------------------------------------------------------------  
// Function: void displayNewScreen(uint32_t currFrame)
// Description: Initializes the screen with the current frame's walls and exits.
//              The function uses the current frame index to access and draw walls and exits.
//------------------------------------------------------------------------------  
void LCD::displayNewScreen(uint32_t currFrame){
    // Draw new
    uint32_t n = 0; // First N walls are perimiater. (This feature is not in use, hence n=0)
    for(int i = n; i < frames[currFrame].wallsIndex; i ++){
        ST7735_FillRect(frames[currFrame].walls[i].TLx, frames[currFrame].walls[i].TLy, (frames[currFrame].walls[i].BRx-frames[currFrame].walls[i].TLx), (frames[currFrame].walls[i].BRy-frames[currFrame].walls[i].TLy), this->wallColor);
    }
    for(int i = 0; i < frames[currFrame].exitsIndex; i ++){
        ST7735_FillRect(frames[currFrame].exits[i].TLx, frames[currFrame].exits[i].TLy, (frames[currFrame].exits[i].BRx-frames[currFrame].exits[i].TLx), (frames[currFrame].exits[i].BRy-frames[currFrame].exits[i].TLy), this->exitColor);
    }
}

//------------------------------------------------------------------------------  
// Function: void frameShift(Player& p1)
// Description: Handles the transition between frames by erasing elements from the old 
//              frame and drawing elements for the new frame. 
//------------------------------------------------------------------------------  
void LCD::frameShift(Player& p1){
    // Erase Walls and Exits
    uint32_t n = 0; // First N walls are perimiater. (This feature is not in use, hence n=0)
    for(int i = n; i < frames[oldFrame].wallsIndex; i ++){
        ST7735_FillRect(frames[oldFrame].walls[i].TLx, frames[oldFrame].walls[i].TLy, (frames[oldFrame].walls[i].BRx-frames[oldFrame].walls[i].TLx), (frames[oldFrame].walls[i].BRy-frames[oldFrame].walls[i].TLy), this->backgroundColor);
    }
    for(int i = 0; i < frames[oldFrame].exitsIndex; i ++){
        ST7735_FillRect(frames[oldFrame].exits[i].TLx, frames[oldFrame].exits[i].TLy, (frames[oldFrame].exits[i].BRx-frames[oldFrame].exits[i].TLx), (frames[oldFrame].exits[i].BRy-frames[oldFrame].exits[i].TLy), this->backgroundColor);
    }

    // Erase Player
    ST7735_FillRect(p1.prevX, p1.prevY, 8, 8, this->backgroundColor);
    
    // Erase Enemies
    for(int i = 0; i < 3; i ++){
        if(enemys[i].frame == this->oldFrame){
            ST7735_FillRect(enemys[i].x, enemys[i].y, 8, 8, this->backgroundColor);
        }
    }

    // Draw Walls and Exits
    for(int i = n; i < frames[newFrame].wallsIndex; i ++){
        ST7735_FillRect(frames[newFrame].walls[i].TLx, frames[newFrame].walls[i].TLy, (frames[newFrame].walls[i].BRx-frames[newFrame].walls[i].TLx), (frames[newFrame].walls[i].BRy-frames[newFrame].walls[i].TLy), this->wallColor);
    }
    for(int i = 0; i < frames[newFrame].exitsIndex; i ++){
        ST7735_FillRect(frames[newFrame].exits[i].TLx, frames[newFrame].exits[i].TLy, (frames[newFrame].exits[i].BRx-frames[newFrame].exits[i].TLx), (frames[newFrame].exits[i].BRy-frames[newFrame].exits[i].TLy), this->exitColor);
    }

    // Clear Frameshift Semaphor (did the frame shift)
    this->FrameShift = false;

}


//------------------------------------------------------------------------------  
// Function: void displayPlayer(Player& p1)
// Description: Displays the player on the screen using the bitmap representation. 
// Note: The player has a black buffer around them, so as they move, they erase
//       themselves.
//------------------------------------------------------------------------------  
void LCD::displayPlayer(Player& p1){
    ST7735_DrawBitmap(p1.x-4, p1.y+11, p1.image, p1.w, p1.h);
}

//------------------------------------------------------------------------------  
// Function: void displayShots(uint32_t currentFrame)
// Description: Draws all valid shots on the screen for the specified frame.
//------------------------------------------------------------------------------  
void LCD::displayShots(uint32_t currentFrame){
    for(int i = 0; i < 4; i ++){
        if((shots[i].frame == currentFrame) && (shots[i].valid == true)){
            ST7735_FillRect(shots[i].TLx, shots[i].TLy, (shots[i].BRx - shots[i].TLx), (shots[i].BRy - shots[i].TLy), this->shotColor);
        }
    }
}

//------------------------------------------------------------------------------  
// Function: void clearShots(uint32_t currentFrame)
// Description: Clears all valid shots from the screen for the given frame by drawing 
//              over them with the background color. Marks shots as invalid to indicate 
//              they are no longer active.
//------------------------------------------------------------------------------  
void LCD::clearShots(uint32_t currentFrame){
    for(int i = 0; i < 4; i ++){
        if((shots[i].frame == currentFrame) && (shots[i].valid == true)){
            ST7735_FillRect(shots[i].TLx, shots[i].TLy, (shots[i].BRx - shots[i].TLx), (shots[i].BRy - shots[i].TLy), this->backgroundColor);
            shots[i].valid = false; // Clear valid semaphore (shot has already been shot, and is therefore no longer active)
        }
    }
}


//------------------------------------------------------------------------------  
// Function: void displayEnemies(uint32_t currFrame)
// Description: Displays all enemies for the current frame. And erase enemies that 
//              just changed frames.
//------------------------------------------------------------------------------  
void LCD::displayEnemies(uint32_t currFrame){
    for(int i = 0; i < 3; i ++){
        if(enemys[i].changedFrames){
            ST7735_FillRect(enemys[i].prevX, enemys[i].prevY, 8, 8, this->backgroundColor);
        }
        if(enemys[i].frame == currFrame){
            ST7735_DrawBitmap(enemys[i].x-4, enemys[i].y+11, enemys[i].image, enemys[i].w, enemys[i].h);
        }
    }
}

//------------------------------------------------------------------------------  
// Function: void clearSpecificEnemy(Enemy& e1)
// Description: Clears a specific enemy. This function is not being used.
//------------------------------------------------------------------------------  
void LCD::clearSpecificEnemy(Enemy& e1){
    ST7735_FillRect(e1.prevX, e1.prevY, 8, 8, this->backgroundColor);
}
