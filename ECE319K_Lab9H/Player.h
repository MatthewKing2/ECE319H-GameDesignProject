
//------------------------------------------------------------------------------
// File: Player.h
// Author: Matthew King
// Description: Header file for the Player class. Manages the player's position, 
//              dimensions, appearance, and state. Provides methods for movement,
//              collision detection, and error correction. Assumes external 
//              dependencies for frame data and joystick inputs.
// Dependencies: Frame.h
//------------------------------------------------------------------------------

#ifndef PLAYER_H_
#define PLAYER_H_
#include <stdint.h>
#include "Frame.h"

//------------------------------------------------------------------------------
// Class: Player
// Description: Represents a player in the game with properties such as position,
//              dimensions, color, appearance, and state. Provides functions to 
//              handle movement, collision detection, and state management. 
//              The player's movement and interactions are influenced by joystick
//              inputs and frame data.
//------------------------------------------------------------------------------
class Player{ 
  public: 
    int32_t x;              // Player X position 
    int32_t y;              // Player Y position
    int32_t prevX;          // Previous X position of the player 
    int32_t prevY;          // Previous Y position of the player
    uint16_t h, w;          // Player width and height 
    uint32_t color;         // Player color
    const uint16_t* image;  // Sprite image representing the player
    bool alive;             // Indicates if the player is alive
    bool murder;            // Indicates if the player is the murderer

  public:
    //--------------------------------------------------------------------------
    // Function: Player(int32_t x, int32_t y, const uint16_t* image, bool murder)
    // Description: Constructor initializes the player's position, sprite, and 
    //              state, including whether the player is the murderer.
    // Parameters:
    //    - x: Initial X position of the player.
    //    - y: Initial Y position of the player.
    //    - image: Sprite image for the player.
    //    - murder: State indicating if the player is the murderer.
    //--------------------------------------------------------------------------
    Player(int32_t x, int32_t y, const uint16_t* image, bool murder);

    //--------------------------------------------------------------------------
    // Movement Functions
    //--------------------------------------------------------------------------

      //--------------------------------------------------------------------------
      // Description: Moves the player in a linear fashion based on joystick input and frame index.
      void moveLinear(uint32_t joystickX, uint32_t joystickY, uint32_t frameIndex);

      //--------------------------------------------------------------------------
      // Description: Moves the player based on exponential joystick input.
      void moveExpo(uint32_t joystickX, uint32_t joystickY);

      //--------------------------------------------------------------------------
      // Description: Determines the maximum allowed movement for the player.
      void maxMove(int32_t deltaX, int32_t deltaY, uint32_t currFrameIndex);

    //--------------------------------------------------------------------------
    // Collision Functions
    //--------------------------------------------------------------------------

      //--------------------------------------------------------------------------
      // Description: Checks if the player is touching an exit and updates the new frame index.
      // Parameters:
      //    - currFrameIndex: Current frame index.
      //    - newFrameIndex: Pointer to store the new frame index if the player is touching an exit.
      // Returns: True if touching exit, false otherwise.
      bool touchingExit(uint32_t currFrameIndex, uint32_t* newFrameIndex);

      //--------------------------------------------------------------------------
      // Description: Calculates the distance to the closest wall in the X direction.
      void distToClosestWallX(int32_t x, int32_t y, uint32_t frameIndex);

      //--------------------------------------------------------------------------
      // Description: Calculates the distance to the closest wall in the Y direction.
      void distToClosestWallY(int32_t x, int32_t y, uint32_t frameIndex);

      //--------------------------------------------------------------------------
      // Description: Checks if the player is touching a laser.
      // Returns: True if touching a laser, false otherwise.
      bool touchingLaser(uint32_t currFrameIndex);

    //--------------------------------------------------------------------------
    // Getters
    //--------------------------------------------------------------------------

      //--------------------------------------------------------------------------
      // Description: Gets the current X position of the player.
      // Description: Gets the current Y position of the player.
      uint32_t x_position(void); 

      uint32_t y_position(void); 

};
#endif


