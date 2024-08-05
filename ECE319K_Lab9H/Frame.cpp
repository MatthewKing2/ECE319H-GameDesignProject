
#include <ti/devices/msp/msp.h>
#include "Frame.h"
#include "Exit.h"
#include "Wall.h"

//------------------------------------------------------------------------------
// Function: Frame::Frame()
// Description: Default constructor for the Frame class. Initializes the walls
//              and exits arrays and their respective indices to zero.
//------------------------------------------------------------------------------
Frame::Frame(){
    // Index just used for adding to the array
    this->wallsIndex = 0;
    this->exitsIndex = 0;

    // Init the arrays to the per-defined size 
    Exit exits[NumberExits];
    Wall walls[NumberWalls];
}

//------------------------------------------------------------------------------
// Function: void Frame::InitWall(int32_t TLx, int32_t TLy, int32_t BRx, int32_t BRy)
// Description: Adds a wall to the frame at the specified coordinates. 
// Note: The assignment uses the default assignment operator,
//       which is a shallow copy. This is acceptable as long as the `Wall` class
//       does not manage dynamic resources or pointers that need deep copying.
//------------------------------------------------------------------------------
// Add a wall to the array 
void Frame::InitWall(int32_t TLx, int32_t TLy, int32_t BRx, int32_t BRy){

    // Simple error prevention 
    if(this->wallsIndex >= NumberWalls){return;}

    // Make a wall and add it to the array 
    walls[this->wallsIndex] = Wall(TLx, TLy, BRx, BRy);
    this->wallsIndex ++;
    
}

//------------------------------------------------------------------------------
// Function: void Frame::InitExit(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame)
// Description: Adds an exit to the frame with the specified coordinates and new
//              player position/frame index upon exiting. 
// Note: Similar to InitWall, the assignment uses the default assignment operator 
//       for the `Exit` class, which is a shallow copy. 
//------------------------------------------------------------------------------
void Frame::InitExit(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame){      // Constructor 

    // Simple error prevention 
    if(this->exitsIndex >= NumberExits){return;}

    // Make a wall and add it to the array 
    exits[this->exitsIndex] = Exit(TRx, TRy, BRx, BRy, newPx, newPy, newPFrame);
    this->exitsIndex ++;

}
