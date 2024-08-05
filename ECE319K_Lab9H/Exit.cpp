
//------------------------------------------------------------------------------
// File: Exit.cpp
// Author: Matthew King
// Description: Implementation of the Exit class, which represents an exit point 
//              in the game map. 
// Dependencies: Exit.h
//------------------------------------------------------------------------------

#include <ti/devices/msp/msp.h>
#include "Exit.h"

//------------------------------------------------------------------------------
// Function: Exit::Exit(int32_t TLx, int32_t TLy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame)
// Description: This constructor is used when adding walls to a frame 
//------------------------------------------------------------------------------
Exit::Exit(int32_t TLx, int32_t TLy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame){
  this->TLx = TLx;
  this->TLy = TLy;
  this->BRx = BRx;
  this->BRy = BRy;

  this->newPlayerX = newPx;
  this->newPlayerY = newPy;
  this->newPlayerFrame = newPFrame;
}

//------------------------------------------------------------------------------
// Function: Exit::Exit()
// Description: Default constructor is not used.  
//------------------------------------------------------------------------------
Exit::Exit(){
  this->TLx = 0;
  this->TLy = 0;
  this->BRx = 0;
  this->BRy = 0;

  this->newPlayerX = 0;
  this->newPlayerY = 0;
  this->newPlayerFrame = 0;
}

//------------------------------------------------------------------------------
// Function: bool Exit::touching(int32_t x, int32_t y, uint32_t h, uint32_t w)
// Description: 
// - The check is performed by verifying if the rectangle is NOT within the bounds of the
//   exit; if it is not, the function returns false, otherwise true.
// - Note: The coordinate system used for the game has has origin (0,0) in the top left 
//   corner of the screen. Y-coordinates increase as you go down the screen.
//------------------------------------------------------------------------------
bool Exit::touching(int32_t x, int32_t y, uint32_t h, uint32_t w){

    if( ((x+w < TLx)||(x > BRx))   ||   ((y+h < TLy)||(y > BRy)) ){
      return false;
    }
    return true;

}
