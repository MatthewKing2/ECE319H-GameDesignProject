
#include <ti/devices/msp/msp.h>
#include "Frame.h"


// constructor, invoked on creation of class
// m and b are linear calibration coefficents
Frame::Frame(uint32_t frameNumber, uint32_t numWalls, uint32_t numExits){

    // Which frame is this
    this-frameNumber = frameNumber;

    // Actual number of walls and exits
    this->numWalls = numWalls+4;        // going to add 4 primiter walls
    this->numExits = numExits;

    // Index just used for adding to the array on the heap
    this->wallsIndex = 0;
    this->exitsIndex = 0;

    // Now have to make the arrays on the heap
    this->walls = new Wall[this->numWalls];
    this->exits = new Exit[this->numExits];

    // Add perimiter walls (todo)
    uint32_t width = 80;
    uint32_t height = 80;
    uint32_t originOffset = 10;
    InitWall(originOffset,originOffset, originOffset+width,originOffset+1); // Top 
    InitWall(originOffset,originOffset+height, originOffset+width,originOffset+height-1); // Bottom 
    // InitWall(10,13, 30,12); // Right
    // InitWall(10,10, 30,12); // Left
}

// Add a wall to the array 
void Frame::InitWall(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy){

    // Simple error prevention 
    if(this->wallsIndex >= numWalls){return;}

    // Make a wall and add it to the array 
    walls[this->wallsIndex] = Wall(TRx, TRy, BRx, BRy);
        // Not sure if this works 
        // Not sure if a need a destucotr for wall
        // Ask someone about this 

}

// Add an exit to the array 
void Frame::InitExit(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy){

    // Simple error prevention 
    if(this->exitsIndex >= numExits){return;}

    // Make a wall and add it to the array 
    exits[this->exitsIndex] = Exit(TRx, TRy, BRx, BRy);
        // Not sure if this works 
        // Not sure if a need a destucotr for wall
        // Ask someone about this 

}

