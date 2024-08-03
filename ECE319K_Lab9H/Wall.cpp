
#include <ti/devices/msp/msp.h>
#include "Wall.h"

// constructor, invoked on creation of class
Wall::Wall(int32_t TLx, int32_t TLy, int32_t BRx, int32_t BRy){
  this->TLx = TLx;
  this->TLy = TLy;
  this->BRx = BRx;
  this->BRy = BRy;
}

// Default Constuctor 
Wall::Wall(){
  this->TLx = 0;
  this->TLy = 0;
  this->BRx = 0;
  this->BRy = 0;
}


bool Wall::fastTouching(int32_t x, int32_t y, int32_t h, int32_t w){

    // Note: 
    // TLx, TLy -*
    //  |        | 
    //  |        |
    //  * --- BRx, BRy 
    // BUT, BRy is greater in value than TLy b/c LCD's
    // origin is top left 

    // Easier to check if NOT touching 
      // See if X is out of bounds
      // See if Y is out of bounds
    if( ((x+w-1 < TLx)||(x > BRx))   ||   ((y+h-1 < TLy)||(y > BRy)) ){
      return false;
    }
    return true;
}





// Sees if some Rectange is touching this Wall 
// Returns
  // True if touching
  // how far into the wall (x and y) you are
  // Note: does not change values if not touching 
  // NOTE: X AND Y ARE ASSUMED TO BE THE TOP LEFT CORDINATES OF THE RECTANGE 
bool Wall::touching(int32_t x, int32_t y, int32_t h, int32_t w, int32_t* Xover, int32_t* Yover){

    // Note: 
    // TLx, TLy -*
    //  |        | 
    //  |        |
    //  * --- BRx, BRy 
    // BUT, BRy is greater in value than TLy b/c LCD's
    // origin is top left 

    // Easier to check if NOT touching 
      // See if X is out of bounds
      // See if Y is out of bounds
    bool touching = true;
    if( ((x+w < TLx)||(x > BRx))   ||   ((y+h < TLy)||(y > BRy)) ){
      touching = false;
    }

    if(touching){
      // Again I need to know if it was an X or a Y collision or both 
      // Determine MaxMoveX 
      int32_t temp1 = TLx-(x+w);
      int32_t temp2 = BRx-x;
      if(temp1 < 0){temp1 *=-1;}
      if(temp2 < 0){temp2 *=-1;}

      if((temp1 < temp2) && temp1 <= 4){
        *Xover = TLx-(x+w);
      }else if(temp2 <= 4){
        *Xover = BRx - x;
      }

      // Determine MaxMoveY
      temp1 = BRy-y;
      temp2 = TLy-(y+h);
      if(temp1 < 0){temp1 *=-1;}
      if(temp2 < 0){temp2 *=-1;}

      if((temp1 < temp2)&& temp1<=4){
        *Yover = BRy-y; 
      }else if(temp2 <= 4){
        *Yover = TLy-(y+h); 
      }

    }

    return touching;

}

