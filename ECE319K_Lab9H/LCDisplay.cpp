
#include <ti/devices/msp/msp.h>
#include "LCDisplay.h"
#include "../inc/ST7735.h"
#include "LaserShot.h"

extern Frame frames[];
extern Shot shots[];


// Constructor 
LCD::LCD(){
    this->FrameShift = false;   // Semphaor 
    this->oldFrame = 0;             // Mailbox 
    this->newFrame = 0;             // Mailbox
    this->DisplayReady = false;  // Not ready yet
    // Colors for stuff and things
    this->backgroundColor = ST7735_Color565(0, 0, 0);
    this->wallColor = ST7735_Color565(255, 255, 255);
    this->exitColor = ST7735_Color565(0, 204, 255);
    this->shotColor = ST7735_Color565(255, 0, 0);

}

// Displays the Map Border (contstant across all frames)
// Exits are left open
void LCD::displayNewScreen(){
    // Draw new
    uint32_t n = 0; // first N walls are perimiater, dont show 
    for(int i = n; i < frames[newFrame].wallsIndex; i ++){
        ST7735_FillRect(frames[newFrame].walls[i].TLx, frames[newFrame].walls[i].TLy, (frames[newFrame].walls[i].BRx-frames[newFrame].walls[i].TLx), (frames[newFrame].walls[i].BRy-frames[newFrame].walls[i].TLy), this->wallColor);
    }
    for(int i = 0; i < frames[newFrame].exitsIndex; i ++){
        ST7735_FillRect(frames[newFrame].exits[i].TLx, frames[newFrame].exits[i].TLy, (frames[newFrame].exits[i].BRx-frames[newFrame].exits[i].TLx), (frames[newFrame].exits[i].BRy-frames[newFrame].exits[i].TLy), this->exitColor);
    }
}


// Erase old frame, show new one (uses global Frames arr)
void LCD::frameShift(Player& p1){
    // Erase Old
    uint32_t n = 0; // First N walls are the perimiter, dont need to display those 
    for(int i = n; i < frames[oldFrame].wallsIndex; i ++){
        ST7735_FillRect(frames[oldFrame].walls[i].TLx, frames[oldFrame].walls[i].TLy, (frames[oldFrame].walls[i].BRx-frames[oldFrame].walls[i].TLx), (frames[oldFrame].walls[i].BRy-frames[oldFrame].walls[i].TLy), this->backgroundColor);
    }
    for(int i = 0; i < frames[oldFrame].exitsIndex; i ++){
        ST7735_FillRect(frames[oldFrame].exits[i].TLx, frames[oldFrame].exits[i].TLy, (frames[oldFrame].exits[i].BRx-frames[oldFrame].exits[i].TLx), (frames[oldFrame].exits[i].BRy-frames[oldFrame].exits[i].TLy), this->backgroundColor);
    }
    // Erase Player
    ST7735_FillRect(p1.prevX, p1.prevY, 8, 8, this->backgroundColor);


    // Draw new
    for(int i = n; i < frames[newFrame].wallsIndex; i ++){
        ST7735_FillRect(frames[newFrame].walls[i].TLx, frames[newFrame].walls[i].TLy, (frames[newFrame].walls[i].BRx-frames[newFrame].walls[i].TLx), (frames[newFrame].walls[i].BRy-frames[newFrame].walls[i].TLy), this->wallColor);
    }
    for(int i = 0; i < frames[newFrame].exitsIndex; i ++){
        ST7735_FillRect(frames[newFrame].exits[i].TLx, frames[newFrame].exits[i].TLy, (frames[newFrame].exits[i].BRx-frames[newFrame].exits[i].TLx), (frames[newFrame].exits[i].BRy-frames[newFrame].exits[i].TLy), this->exitColor);
    }

    // Clear Frameshift Semaphor 
    this->FrameShift = false;

}

// All you have to do is fill rectange on the new player X and Y
// Player automatically erases itself (b/c of background buffer)
    // TODO fix this function to draw bit map
    // Can also get rid of player color then lmao
void LCD::displayPlayer(Player& p1){
    ST7735_DrawBitmap(p1.x-4, p1.y+11, p1.image, p1.w, p1.h);
    //ST7735_FillRect(p1.x_position(), p1.y_position(), 8, 8, p1.color);

}




void LCD::displayShots(uint32_t currentFrame){
    for(int i = 0; i < 4; i ++){
        if((shots[i].frame == currentFrame) && (shots[i].valid == true)){
            ST7735_FillRect(shots[i].TLx, shots[i].TLy, (shots[i].BRx - shots[i].TLx), (shots[i].BRy - shots[i].TLy), this->shotColor);
        }
    }
}




void LCD::clearShots(uint32_t currentFrame){
    for(int i = 0; i < 4; i ++){
        if((shots[i].frame == currentFrame) && (shots[i].valid == true)){
            ST7735_FillRect(shots[i].TLx, shots[i].TLy, (shots[i].BRx - shots[i].TLx), (shots[i].BRy - shots[i].TLy), this->backgroundColor);
            shots[i].valid = false; // Clear valid semaphore (shot has already been shot)
        }
    }
}
