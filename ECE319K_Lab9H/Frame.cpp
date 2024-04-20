
#include <ti/devices/msp/msp.h>
#include "Frame.h"
#include "Exit.h"
#include "Wall.h"



Frame::Frame(){

    // Index just used for adding to the array on the heap
    this->wallsIndex = 0;
    this->exitsIndex = 0;

    // Init the arrays to the per-defined size 
    Exit exits[NumberExits];            // Uses default constructor (I defined this)
    Wall walls[NumberWalls];            // Uses defulat constructor (I defined this)

    // Add perimiter walls (todo)
    // uint32_t width = 80;
    // uint32_t height = 80;
    // uint32_t originOffset = 10;
    // InitWall(originOffset,originOffset, originOffset+width,originOffset+1); // Top 
    // InitWall(originOffset,originOffset+height, originOffset+width,originOffset+height-1); // Bottom 
    // InitWall(10,13, 30,12); // Right
    // InitWall(10,10, 30,12); // Left
}



// Add a wall to the array 
void Frame::InitWall(int32_t TLx, int32_t TLy, int32_t BRx, int32_t BRy){

    // Simple error prevention 
    if(this->wallsIndex >= NumberWalls){return;}

    // Make a wall and add it to the array 
    walls[this->wallsIndex] = Wall(TLx, TLy, BRx, BRy);
        // Wall is destoryed when it exsits scope (default constructor)
        // Using the default operation overloader for "=" for the wall class
            // This is a shllow copy (not an issue in this case)

    this->wallsIndex ++;

}

// Add an exit to the array 
void Frame::InitExit(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame){      // Constructor 

    // Simple error prevention 
    if(this->exitsIndex >= NumberExits){return;}

    // Make a wall and add it to the array 
    exits[this->exitsIndex] = Exit(TRx, TRy, BRx, BRy, newPx, newPy, newPFrame);
        // Exit(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame);      // Constructor 
        // Wall is destoryed when it exsits scope (default constructor)
        // Using the default operation overloader for "=" for the exit class
            // This is a shllow copy (not an issue in this case)

    this->exitsIndex ++;


}

