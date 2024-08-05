
//------------------------------------------------------------------------------
// File: Frame.h
// Author: Matthew King
// Description: Header file for the Frame class. Manages the walls and exits within
//              a game frame. Provides methods for initializing walls and exits.
//              Defines maximum limits for the number of walls and exits.
// Dependencies: Exit.h, Wall.h
//------------------------------------------------------------------------------

#ifndef FRAME_H_
#define FRAME_H_
#include <stdint.h>
#include "Exit.h"
#include "Wall.h"
#define NumberWalls 15  // Max number of walls (including the perimiter)
#define NumberExits 5   // Max number of exits 

//------------------------------------------------------------------------------
// Class: Frame
// Description: Represents a frame (room) in the game with properties for managing 
//              walls and exits. Includes functionality for initializing wall and exit
//              objects within the frame.
//------------------------------------------------------------------------------
class Frame{ 
  public:
    Exit exits[NumberExits];            // Array of exit objects within the frame
    uint32_t exitsIndex;                // Index tracking the number of exits
    Wall walls[NumberWalls];            // Array of wall objects within the frame
    uint32_t wallsIndex;                // Index tracking the number of walls

  public: 
    //--------------------------------------------------------------------------
    // Function: Frame()
    // Description: Default constructor initializes the frame. Does not make any 
    //              wall or exits.
    //--------------------------------------------------------------------------
    Frame();

    //--------------------------------------------------------------------------
    // Function: void InitWall(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy)
    // Description: Initializes a wall within the frame using the specified 
    //              top-right and bottom-right coordinates.
    // Parameters:
    //    - TRx: X-coordinate of the top-right corner.
    //    - TRy: Y-coordinate of the top-right corner.
    //    - BRx: X-coordinate of the bottom-right corner.
    //    - BRy: Y-coordinate of the bottom-right corner.
    //--------------------------------------------------------------------------
    void InitWall(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy); 

    //--------------------------------------------------------------------------
    // Function: void InitExit(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame)
    // Description: Initializes an exit within the frame using the specified 
    //              top-right and bottom-right coordinates. Also defines which 
    //              frame a player will move to when they touch this exit, and
    //              their new x,y position in that frame.
    // Parameters:
    //    - TRx: X-coordinate of the top-right corner.
    //    - TRy: Y-coordinate of the top-right corner.
    //    - BRx: X-coordinate of the bottom-right corner.
    //    - BRy: Y-coordinate of the bottom-right corner.
    //    - newPx: New X position for the player upon exiting.
    //    - newPy: New Y position for the player upon exiting.
    //    - newPFrame: New frame index for the player upon exiting.
    //--------------------------------------------------------------------------
    void InitExit(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame);

};

#endif  // FRAME_H_ 
