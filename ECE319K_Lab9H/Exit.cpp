
#include <ti/devices/msp/msp.h>
#include "Exit.h"



// constructor, invoked on creation of class
Exit::Exit(int32_t TLx, int32_t TLy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame){      // Constructor 
  this->TLx = TLx;
  this->TLy = TLy;
  this->BRx = BRx;
  this->BRy = BRy;

  this->newPlayerX = newPx;
  this->newPlayerY = newPy;
  this->newPlayerFrame = newPFrame;

}

// Default Constructor
Exit::Exit(){

  this->TLx = 0;
  this->TLy = 0;
  this->BRx = 0;
  this->BRy = 0;

  this->newPlayerX = 0;
  this->newPlayerY = 0;
  this->newPlayerFrame = 0;

}



// Sees if some X and Y are touching the wall 
bool Exit::touching(uint32_t x, uint32_t y){

    // Casting to singed just to be safe 
    // (values should always be pos just dont want the compares to mess up)
    x = (int32_t)x;
    y = (int32_t)y;

    // Note: 
    // TLx, TLy -*
    //  |        | 
    //  |        |
    //  * --- BRx, BRy 
    // BUT, BRy is greater in value than TLy b/c LCD's
    // origin is top left 

    int8_t playerWH = 8+4; // Player Width (and Hight) + buffer around them 

    // Easier to check if NOT touching 
      // See if X is out of bounds
      // See if Y is out of bounds
    if( ((x+playerWH < TLx)||(x > BRx))   ||   ((y+playerWH < TLy)||(y > BRy)) ){
      return false; // not touching
    }
    return true; // touching 


}


