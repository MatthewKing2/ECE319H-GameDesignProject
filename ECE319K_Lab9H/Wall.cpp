
#include <ti/devices/msp/msp.h>
#include "Wall.h"


// constructor, invoked on creation of class
Wall::Wall(int32_t TLx, int32_t TLy, int32_t BRx, int32_t BRy){
  this->TLx = TLx;
  this->TLy = TLy;
  this->BRx = BRx;
  this->BRy = BRy;
}

// Sees if some Player X and Y are touching the wall 
bool Wall::touching(uint32_t x, uint32_t y){

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

    int8_t playerWH = 8+(2*4); // Player Width (and Hight) + (buffer around them * 2 sides)

    // Easier to check if NOT touching 
      // See if X is out of bounds
      // See if Y is out of bounds
    if( ((x+playerWH < TLx)||(x > BRx))   ||   ((y+playerWH < TLy)||(y > BRy)) ){
      return false; // not touching
    }
    return true; // touching 

}

