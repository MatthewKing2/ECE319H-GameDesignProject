
//------------------------------------------------------------------------------
// File: Exit.h
// Author: Matthew King
// Description: Header file for the Exit class, representing an exit point within 
//              a frame.
//------------------------------------------------------------------------------

#ifndef EXIT_H_
#define EXIT_H_
#include <stdint.h>

//------------------------------------------------------------------------------
// Class: Exit
// Description: Represents an exit in the game. Includes data members for the
//              top-left and bottom-right coordinates of the exit, as well as the
//              new player position and frame upon exiting.
//------------------------------------------------------------------------------
class Exit{ 
  public: 
    int32_t TLx;            // Top right X 
    int32_t TLy;            // Top right Y
    int32_t BRx;            // Bottom left X 
    int32_t BRy;            // Bottom left Y
    uint32_t newPlayerX;    // New Player X 
    uint32_t newPlayerY;    // New Player Y
    uint32_t newPlayerFrame;// New Player Frame (0-8)


  public: 
    //------------------------------------------------------------------------------
    // Function: Exit::Exit(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame)
    // Description: Constructor that initializes an Exit object with specified coordinates 
    //              and new player attributes. 
    // Parameters:
    //    - TRx: Top-right X coordinate of the exit.
    //    - TRy: Top-right Y coordinate of the exit.
    //    - BRx: Bottom-right X coordinate of the exit.
    //    - BRy: Bottom-right Y coordinate of the exit.
    //    - newPx: New X position for the player upon using the exit.
    //    - newPy: New Y position for the player upon using the exit.
    //    - newPFrame: New frame index for the player upon using the exit.
    //------------------------------------------------------------------------------
    Exit(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame);

    //------------------------------------------------------------------------------
    // Function: Exit::Exit()
    // Description: Default constructor for the Exit class. Initializes the data members
    //              to default values. 
    //------------------------------------------------------------------------------
    Exit();

    //------------------------------------------------------------------------------
    // Function: bool Exit::touching(int32_t x, int32_t y, uint32_t h, uint32_t w)
    // Description: Checks if a given rectangle (defined by its top-left corner coordinates 
    //              and dimensions) is touching or intersecting with this exit.
    // Parameters:
    //    - x: X coordinate of the top-left corner of the rectangle.
    //    - y: Y coordinate of the top-left corner of the rectangle.
    //    - h: Height of the rectangle.
    //    - w: Width of the rectangle.
    // Returns: A boolean value indicating whether the specified rectangle is touching
    //          or intersecting the exit. Touching = True, Not Touching = False.
    //------------------------------------------------------------------------------
    bool touching(int32_t x, int32_t y, uint32_t h, uint32_t w);

};

#endif
