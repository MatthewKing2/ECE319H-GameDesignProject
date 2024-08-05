
//------------------------------------------------------------------------------
// File: Wall.h
// Author: Matthew King
// Description: Header file for the Wall class, representing walls within the 
//              game map. This class includes data members to define the 
//              wall's boundaries and methods to check if a player or object 
//              is touching the wall.
//------------------------------------------------------------------------------

#ifndef WALL_H_
#define WALL_H_
#include <stdint.h>

//------------------------------------------------------------------------------
// Class: Wall
// Description: Class class includes data members to define the wall's boundaries 
//              and methods to check if a player or object is touching the wall.
//------------------------------------------------------------------------------
class Wall{ 
  public:
    int32_t TLx;           // Top right X 
    int32_t TLy;           // Top right Y
    int32_t BRx;           // Bottom left X 
    int32_t BRy;           // Bottom left Y

  public: 
    //------------------------------------------------------------------------------
    // Function: Wall::Wall(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy)
    // Description: Constructor that initializes a wall with the specified top-left (TRx, TRy)
    //              and bottom-right (BRx, BRy) coordinates.
    //------------------------------------------------------------------------------
    Wall(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy);

    //------------------------------------------------------------------------------
    // Function: Wall::Wall()
    // Description: Default constructor (not used).
    //------------------------------------------------------------------------------
    Wall();

    //------------------------------------------------------------------------------
    // Function: bool Wall::touching(int32_t x, int32_t y, int32_t h, int32_t w, int32_t* Xover, int32_t* Yover)
    // Description: Checks if a rectangle with top-left corner (x, y) and dimensions (h, w) 
    //              is touching the wall. It calculates the overlap distances, if any, and 
    //              stores them in Xover and Yover.
    // Returns:
    // - True if the rectangle is touching the wall, 
    // - False otherwise.
    // - Xover: How far rectangle needs to move so its not touching in the wall the X direction
    // - Yover: How far rectangle needs to move so its not touching in the wall the Y direction
    //------------------------------------------------------------------------------
    bool touching(int32_t x, int32_t y, int32_t h, int32_t w, int32_t* Xover, int32_t* Yover);

    //------------------------------------------------------------------------------
    // Function: bool Wall::fastTouching(int32_t x, int32_t y, int32_t h, int32_t w)
    // Description: Quickly checks if a rectangle with top-left corner (x, y) and dimensions 
    //              (h, w) is touching the wall. This function does not calculate the overlap 
    //              distances.
    // Returns:
    // - True if the rectangle is touching the wall, 
    // - False otherwise.
    //------------------------------------------------------------------------------
    bool fastTouching(int32_t x, int32_t y, int32_t h, int32_t w);

};

#endif
