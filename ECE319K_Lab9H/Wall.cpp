


#include <ti/devices/msp/msp.h>
#include "Wall.h"

//------------------------------------------------------------------------------
// Wall::Wall(int32_t TLx, int32_t TLy, int32_t BRx, int32_t BRy)
// Constructor initializes a wall with given coordinates for the top-left (TLx, TLy)
// and bottom-right (BRx, BRy) corners.
//------------------------------------------------------------------------------
Wall::Wall(int32_t TLx, int32_t TLy, int32_t BRx, int32_t BRy){
  this->TLx = TLx;
  this->TLy = TLy;
  this->BRx = BRx;
  this->BRy = BRy;
}

//------------------------------------------------------------------------------
// Wall::Wall(int32_t TLx, int32_t TLy, int32_t BRx, int32_t BRy)
// Default Constructor (not used) 
//------------------------------------------------------------------------------
Wall::Wall(){
  this->TLx = 0;
  this->TLy = 0;
  this->BRx = 0;
  this->BRy = 0;
}

//------------------------------------------------------------------------------
// bool Wall::fastTouching(int32_t x, int32_t y, int32_t h, int32_t w)
// Checks if a rectangle with top-left corner (x, y) and dimensions (h, w) 
// is touching the wall. 
//------------------------------------------------------------------------------
bool Wall::fastTouching(int32_t x, int32_t y, int32_t h, int32_t w){

   if( ((x+w-1 < TLx)||(x > BRx))   ||   ((y+h-1 < TLy)||(y > BRy)) ){
      return false;
    }
    return true;

}

//------------------------------------------------------------------------------
// Function: bool Wall::touching(int32_t x, int32_t y, int32_t h, int32_t w, int32_t* Xover, int32_t* Yover)
// - The function calculates the amount the rectangle is "into" the wall in both 
//   x and y directions, considering the screen's coordinate system where (0, 0) 
//   is at the top-left corner. It distinguishes between x and y collisions and 
//   ensures the overlap distance does not exceed 4 pixels.
//------------------------------------------------------------------------------
bool Wall::touching(int32_t x, int32_t y, int32_t h, int32_t w, int32_t* Xover, int32_t* Yover){

    bool touching = true;
    if( ((x+w < TLx)||(x > BRx))   ||   ((y+h < TLy)||(y > BRy)) ){
      touching = false;
    }

    if(touching){

      // What this confusing code is doing: 
        // temp1 and temp2 are used to calculate the minimal overlap between the rectangle and the wall in both x and y directions. 
        // These calculations measure the extent of the overlap. The values are constrained to a maximum of 4 pixels, specific to the game's mechanics.

      // Determine MaxMoveX 
        // - temp1: Distance from the right edge of the rectangle to the left edge of the wall.
        // - temp2: Distance from the left edge of the rectangle to the right edge of the wall.
        // The smaller absolute value indicates the minimal overlap distance, which is assigned to Xover.
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
        //   - temp1: Distance from the top edge of the rectangle to the bottom edge of the wall.
        //   - temp2: Distance from the bottom edge of the rectangle to the top edge of the wall.
        //   The smaller absolute value indicates the minimal overlap distance, which is assigned to Yover.
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
