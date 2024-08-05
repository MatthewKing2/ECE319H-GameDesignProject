
//------------------------------------------------------------------------------
// File: Enemy.h
// Author: Matthew King
// Description: Header file for the Enemy class. Represents an enemy in the 
//              game, tracking its position, state, and appearance.
//              Provides functions to update the enemy's position and sprite.
//              For context, the enemy data (position, frame, etc.) is received
//              from another microcontroller, and this class provides methods to 
//              update and manage the enemy based on this external input.
// Dependencies: None
//------------------------------------------------------------------------------

#ifndef ENEMY_H_
#define ENEMY_H_
#include <stdint.h>

//------------------------------------------------------------------------------
// Class: Enemy
// Description: Represents an enemy in the game with properties such as position,
//              dimensions, color, sprite image, and state (alive or dead).
//              Handles updating its position and sprite image.
//------------------------------------------------------------------------------
class Enemy{ 
  public: 
    int32_t x;              // Enemy X position 
    int32_t y;              // Enemy Y position
    uint16_t h,w;           // Enemy Width and Hight 
    uint32_t color;         // Enemy color
    const uint16_t* image;  // What Enemy looks like
    bool alive;             // Is Enemy alive
    uint32_t frame;         // What frame Enemy is in

    //------------------------------------------------------------------------------
    // Mailbox system
    // Description: Variables and flags used to manage the state and changes in the
    //              enemy's position and frame. This system helps track if an enemy
    //              has changed frames and needs to be erased or redrawn on the display.
    //
    //    - prevFrame: The previous frame the enemy was in.
    //    - prevX: The previous X position of the enemy before changing frames.
    //    - prevY: The previous Y position of the enemy before changing frames.
    //    - changedFrames: A flag indicating if the enemy has changed frames. If true,
    //                     the enemy needs to be erased from the previous position 
    //                     and drawn in the new frame.
    //------------------------------------------------------------------------------
    uint32_t prevFrame; 
    int32_t prevX;      
    int32_t prevY;
    bool changedFrames; 

  public: 
    //------------------------------------------------------------------------------
    // Function: Enemy(int32_t x, int32_t y, const uint16_t* image, uint32_t frame)
    // Description: Parameterized constructor for the Enemy class.
    // Parameters:
    //    - x: Initial X position of the enemy.
    //    - y: Initial Y position of the enemy.
    //    - image: Sprite image for the enemy.
    //    - frame: Initial frame the enemy is in.
    //------------------------------------------------------------------------------
    Enemy(int32_t x, int32_t y, const uint16_t* image, uint32_t frame);

    //------------------------------------------------------------------------------
    // Function: Enemy()
    // Description: Default constructor for the Enemy class.
    //------------------------------------------------------------------------------
    Enemy();

    //------------------------------------------------------------------------------
    // Function: void updatePosition(int32_t x, int32_t y, uint32_t frame, bool alive)
    // Description: Updates the position and state of the enemy.
    // Parameters:
    //    - x: New X position of the enemy.
    //    - y: New Y position of the enemy.
    //    - frame: New frame the enemy is in.
    //    - alive: New state indicating if the enemy is alive.
    //------------------------------------------------------------------------------
    void updatePosition(int32_t x, int32_t y, uint32_t frame, bool alive);

    //------------------------------------------------------------------------------
    // Function: void assignSprite(const uint16_t* image)
    // Description: Assigns a new sprite image to the enemy.
    // Parameters:
    //    - image: New sprite image for the enemy.
    //------------------------------------------------------------------------------
    void assignSprite(const uint16_t* image);
};
#endif



