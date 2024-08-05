
//------------------------------------------------------------------------------
// File: LaserShot.cpp
// Author: Matthew King
// Description: Contains the implementation of the Shot class which represents laser 
//              shots in the game. Includes methods for managing shot positions, detecting 
//              collisions, generating new shots, and cleaning up expired shots. 
//              The methods involve simple physics calculations for shot movement and 
//              interactions within the game's environment.
//              Note: Assumes the coordinate system's origin is at the top-left corner of the screen.
//------------------------------------------------------------------------------ 

#ifndef LASERSHOT_H_
#define LASERSHOT_H_
#include <stdint.h>

//------------------------------------------------------------------------------
// Class: Shot
// Description: Laser shots are shot by players at each other to eliminate each other.
//              They reach across the entire map until they hit a wall or exit, and 
//              deal manage to enemy players when they collide (Not all of that
//              functionality is directly implimented here). 
//------------------------------------------------------------------------------ 
class Shot{ 
  public: 
    int32_t TLx;           // Top right X 
    int32_t TLy;           // Top right Y
    int32_t BRx;           // Bottom left X 
    int32_t BRy;           // Bottom left Y
    uint32_t frame;        // What frame (index) is the shot in
    bool valid;            // Semaphor - Is this shot current (valid) or outdates (invalid)

  public:
    //------------------------------------------------------------------------------
    // Function: Shot::Shot(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, bool valid)
    // Description: Parameterized constructor initializes a shot with specified coordinates 
    //              and validity flag. Used primarily for testing purposes to create shots 
    //              with predetermined values.
    //------------------------------------------------------------------------------ 
    Shot(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, bool valid);

    //------------------------------------------------------------------------------
    // Function: Shot::Shot(void)
    // Description: Default constructor initializes a shot with default values. (not used)
    //------------------------------------------------------------------------------ 
    Shot(void);

    //------------------------------------------------------------------------------
    // Function: Shot::touching(int32_t x, int32_t y, uint32_t h, uint32_t w)
    // Description: Determines if the shot intersects with a specified rectangular area.
    //              Checks for overlap between the shot's bounding box and another rectangle 
    //              defined by (x, y) and dimensions (h, w). 
    //------------------------------------------------------------------------------ 
    bool touching(int32_t x, int32_t y, uint32_t h, uint32_t w);

    //------------------------------------------------------------------------------
    // Function: Shot::generate(int32_t x, int32_t y, uint32_t direction, uint32_t frame)
    // Description: Updates the shot's position based on its direction and frame. 
    //              Update's the objects data so the shot exsists from the player / enemy
    //              to the nearest wall in the specified direction. 
    // Note: This function is used to update ("make") shots as players fight 
    //------------------------------------------------------------------------------ 
    void generate(int32_t x, int32_t y, uint32_t direction, uint32_t frame);

    //------------------------------------------------------------------------------
    // Function: Shot::cleanUp(uint32_t currFrame)
    // Description: Cleans up expired shots based on the current frame index.
    //              Validates and removes shots that are no longer active or relevant for the current frame.
    // Note: Not currently being used.
    //------------------------------------------------------------------------------ 
    void cleanUp(uint32_t currFrame);

  };

#endif
