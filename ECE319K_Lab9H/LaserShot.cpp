
//------------------------------------------------------------------------------
// File: LaserShot.cpp
// Author: Matthew King
// Description: Implements the `Shot` class functionalities for managing laser shots 
//              in the game. This includes initializing shots, detecting collisions, 
//              generating new shots based on direction, and updating shot validity.
//              The class integrates with the `Frame` and `Wall` classes to interact 
//              with the game's environment.
// Dependencies: Frame.h, LaserShot.h
//------------------------------------------------------------------------------ 

#include <ti/devices/msp/msp.h>
#include "LaserShot.h"
#include "../inc/ST7735.h"
#include "Frame.h"
extern Frame frames[];

//------------------------------------------------------------------------------
// Function: Shot::Shot(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, bool valid)
// Description: Initializes a shot with default values for coordinates and frame.
//              Used primarily for testing, the shot is set to an invalid frame to
//              prevent errors.
//------------------------------------------------------------------------------ 
Shot::Shot(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, bool valid){
    this->TLx = 0;
    this->TLy = 0;
    this->BRx = 0;
    this->BRy = 0;
    this->frame = 10;
    this->valid = false;
}                           

//------------------------------------------------------------------------------
// Function: Shot::Shot()
// Description: Default constructor initializes "empty" shot. (not used)
//------------------------------------------------------------------------------ 
Shot::Shot(){
    this->TLx = 0;
    this->TLy = 0;
    this->BRx = 0;
    this->BRy = 0;
    this->frame = 10;
    this->valid = false;
}

//------------------------------------------------------------------------------
// Function: Shot::touching(int32_t x, int32_t y, uint32_t h, uint32_t w)
// Description: Checks if the shot intersects with a specified rectangular area by 
//              comparing the shot's bounding box with the given coordinates and dimensions.
//------------------------------------------------------------------------------ 
bool Shot::touching(int32_t x, int32_t y, uint32_t h, uint32_t w){ 
    
    if( ((x+w-1 < TLx)||(x > BRx))   ||   ((y+h-1 < TLy)||(y > BRy)) ){
      return false; 
    }
    return true; 

}


//------------------------------------------------------------------------------
// Function: Shot::generate(int32_t x, int32_t y, uint32_t direction, uint32_t currFrameIndex)
// Description: Generates a new shot based on the player's position and the direction butten,
//              pressed, updating the shot's coordinates until it either hits a wall or exit. 
//              Noteworthy:
//              - Depending on the direction, the shot's bounding box (TLx, TLy, BRx, BRy) 
//                is adjusted either vertically or horizontally.
//              - Uses a loop to incrementally expand the shot's size (in the specified direction)
//                while checking for collisions with walls, utilizing the `Wall::touching` method 
//              - Direction values are mapped as follows: 0 = Up, 1 = Down, 2 = Left, 3 = Right.
//------------------------------------------------------------------------------ 
void Shot::generate(int32_t x, int32_t y, uint32_t direction, uint32_t currFrameIndex){

    // Player Height and Width
    uint32_t w = 8;
    uint32_t h = 8;

    // Not currently using these vars, just for funtion calls :)
    int32_t Xover;
    int32_t Yover;

    // Up generate 
    if(direction == 0){
        // TLy will change 
        this->TLx = x;
        this->TLy = y;
        this->BRx = x+w-1;
        this->BRy = y;
        // Change Height by 4 over and over (width no change)
        bool touching = false;
        int32_t deltaH = 0;
        while(!touching && (deltaH > -128)){
            for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
                touching |= frames[currFrameIndex].walls[i].touching(TLx, TLy+deltaH, -1*deltaH, w, &Xover, &Yover);
            }
            deltaH -= 1;
        }

        this->TLy += deltaH+1; 
    }

    // Down generate 
    else if(direction == 1){
        // BRy will change 
        this->TLx = x;
        this->TLy = y+h-1;
        this->BRx = x+w-1;
        this->BRy = y+h-1;
        // Change Height by 4 over and over (width no change)
        bool touching = false;
        int32_t deltaH = 0;
        while(!touching && (deltaH < 128)){
            for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
                touching |= frames[currFrameIndex].walls[i].touching(this->TLx, this->TLy, deltaH, w, &Xover, &Yover);
            }
            deltaH += 1;
        }

        this->BRy += deltaH-1; 
    }

    // Left generate 
    if(direction == 2){
        // TLx will change 
        this->TLx = x;
        this->TLy = y;
        this->BRx = x;
        this->BRy = y+h-1;
        // Change Height by 4 over and over (width no change)
        bool touching = false;
        int32_t deltaW = 0;
        while(!touching && (deltaW > -128)){
            for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
                touching |= frames[currFrameIndex].walls[i].touching(TLx+deltaW, TLy, h, -1*deltaW, &Xover, &Yover);
            }
            deltaW -= 1;
        }

        this->TLx += deltaW+1; 
    }

    // Right generate 
    else if(direction == 3){
        // BRx will change 
        this->TLx = x+w-1;
        this->TLy = y;
        this->BRx = x+w-1;
        this->BRy = y+h-1;
        // Change Height by 4 over and over (width no change)
        bool touching = false;
        int32_t deltaW = 0;
        while(!touching && (deltaW < 128)){
            for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
                touching |= frames[currFrameIndex].walls[i].touching(TLx, TLy, h, deltaW, &Xover, &Yover);
            }
            deltaW += 1;
        }

        this->BRx += deltaW-1; 
    }
    
    // Update Mailbox info 
        // Update: what frame the shot is and assert that the shot is valid
        // Mailbox: This shot will then be used to see if a player took damage
    this->frame = currFrameIndex;
    this->valid = true;

}
