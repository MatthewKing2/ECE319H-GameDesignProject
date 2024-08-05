
//------------------------------------------------------------------------------
// File: Player.cpp
// Author: Matthew King
// Description: Implementation file for the Player class. Contains definitions 
//              for the Player class member functions, including movement logic, 
//              collision detection, and interaction with external frame and shot 
//              data. Assumes external dependencies for joystick inputs, frames, 
//              and shots.
// Dependencies: Player.h, ST7735.h (Joystick), LaserShot.h
//------------------------------------------------------------------------------

#include <ti/devices/msp/msp.h>
#include "Player.h"
#include "../inc/ST7735.h"
#include "LaserShot.h"
#define playerSize 16 
#define PI 3.14159265
extern Frame frames[];
extern Shot shots[];

//------------------------------------------------------------------------------
// Function: Player::Player(int32_t x, int32_t y, const uint16_t* image, bool murder)
// Description: Initializes the player with specified position, sprite image, and 
//              murderer status. Sets default values for player attributes.
//------------------------------------------------------------------------------
Player::Player(int32_t x, int32_t y, const uint16_t* image, bool murder){
  this->x = x;
  this->y = y;
  this->prevX = 0;
  this->prevX = 0;
  this->h = playerSize;
  this->w = playerSize;
  this->color = ST7735_Color565(255, 0, 0);
  this->image = image;
  this->murder = false;
  this->alive = true;
}

//------------------------------------------------------------------------------
// Function: bool Player::touchingLaser(uint32_t currFrameIndex)
// Description: Checks if any of the enemy shots (indices 1 to 3, my shot is index 
//              0) are currently touching the player. Returns true if any valid 
//              enemy shot is touching the player.
// Note: Uses shots array (globally accessible) and sees if valid enemy shots are 
//       (idenified by their frame index) are in collision with the player.
//------------------------------------------------------------------------------
bool Player::touchingLaser(uint32_t currFrameIndex){
    bool touching = false;
    for(int i = 1; i < 4; i++){ 
        if((shots[i].valid) && (shots[i].frame == currFrameIndex)){ 
            touching |= shots[i].touching(this->x,this->y,8,8);
        }
    }
    return touching;
}


//------------------------------------------------------------------------------
// Function: bool Player::touchingExit(uint32_t currFrameIndex, uint32_t* newFrameIndex)
// Description: Determines if the player is touching an exit based on the global
//              frames array. If touching returns true and updates the frame, player position.
// Parameters:
//    - newFrameIndex: Pointer to store the new frame index if an exit is touched.
// Note: Uses frame data to update player position and frame index upon touching an exit.
//       Sets `newFrameIndex` to 10 if no exit being touched, so that if (for some reason)
//       the player tries to change frames when not touching an exit the program will die. 
//       (index out of range error b/c only 9 frames).
//------------------------------------------------------------------------------
bool Player::touchingExit(uint32_t currFrameIndex, uint32_t* newFrameIndex){
    // See if player is touching 
    bool touching = false;
    uint32_t touchingIndex = 0; 

    while((touchingIndex <= frames[currFrameIndex].exitsIndex) && !touching){
        touching = frames[currFrameIndex].exits[touchingIndex].touching(this->x-4, this->y-4, 16, 16);
        touchingIndex ++;
    }

    // If touching, change frames and player X, Y 
    touchingIndex --;   // offset the ++ after the correct index was found
    if(touching){
        *newFrameIndex = frames[currFrameIndex].exits[touchingIndex].newPlayerFrame;
        this->prevX = this->x;
        this->prevY = this->y;
        this->x = frames[currFrameIndex].exits[touchingIndex].newPlayerX;
        this->y = frames[currFrameIndex].exits[touchingIndex].newPlayerY;
        return true;
    }

    // If not touching, return false
    *newFrameIndex = 10; //Index out of range error if used (intentional)
    return false;
}

//------------------------------------------------------------------------------
// Function: void Player::maxMove(int32_t deltaX, int32_t deltaY, uint32_t currFrameIndex)
// Description: Calculates and updates the maximum possible movement of the player 
//              so that player moves as close as possible to walls without overllaping them.
// Parameters:
//    - deltaX: Desired change in X position.
//    - deltaY: Desired change in Y position.
// Note: Adjusts movement to ensure player does not overlap walls, rounding movements
//       when necessary to avoid exceeding bounds.
//------------------------------------------------------------------------------
void Player::maxMove(int32_t deltaX, int32_t deltaY, uint32_t currFrameIndex){

    // How far over a boundary the desired movment is
    int32_t Xover = 0;
    int32_t Yover = 0;

    // Find the values by looking at all walls in frame
    for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
        frames[currFrameIndex].walls[i].touching(this->x + deltaX -4, this->y + deltaY -4, 16, 16, &Xover, &Yover);
    }

    // Since Xover & Yover are how far player must move to get out of the wall, when added with deltaX,Y value may be 
    // greater than 4 (the greatest allowed player movment). Therefore, need to round down to 4 (but mod 5 is only for 
    // positive numbers), therefore this solution:
    bool outOfRangeX = false;
    bool outOfRangeY = false;

    // Change X
    if( (deltaX + Xover) < -4 ){
        this->x -= 4; 
        outOfRangeX = true;
    }else if((deltaX + Xover) > 4){
        this->x += 4;
        outOfRangeX = true;
    }
    if(!outOfRangeX){this->x += (deltaX + Xover);}

    // Change Y
    if( (deltaY + Yover) < -4 ){
        this->y -= 4; 
        outOfRangeY = true;
    }else if((deltaY + Yover) > 4){
        this->y += 4;
        outOfRangeY = true;
    }
    if(!outOfRangeY){this->y += (deltaY + Yover);}

}

//------------------------------------------------------------------------------
// Function: void Player::moveLinear(uint32_t joyStickX, uint32_t joyStickY, uint32_t currFrameIndex)
// Description: Moves the player based on joystick input using a linear approach, 
//              considering walls and adjusting movement accordingly.
// Parameters:
//    - joyStickX: X-axis joystick input.
//    - joyStickY: Y-axis joystick input.
//    - currFrameIndex: Current frame index used for wall detection.
// Note: Adjusts joystick inputs and uses `maxMove` to handle movement while avoiding walls.
//------------------------------------------------------------------------------
void Player::moveLinear(uint32_t joyStickX, uint32_t joyStickY, uint32_t currFrameIndex){

    //Change origin (and invert x,y)
    int32_t x = joyStickX - 2048 ;
    int32_t y = joyStickY - 2048 ;
    y *= -1;
    x *= -1;

    // The max joystick value (with new origin) is 2048. 
    // Want a max of 4 pixle movment at a time 
    // 2048/x = 4. x is 512
    x /= 512;
    y /= 512;

    // Use the maxMove function to handle wall collisions
    maxMove(x, y, currFrameIndex);

}

//------------------------------------------------------------------------------
// Function: void Player::moveExpo(uint32_t joyStickX, uint32_t joyStickY)
// Description: Moves the player based on joystick input using an exponential approach.
// Parameters:
//    - joyStickX: X-axis joystick input.
//    - joyStickY: Y-axis joystick input.
// Note: This function is not used
// Note: Squared joystick input values to achieve exponential movement, then scaled
//       to update player position. Inverts Y movement.
//------------------------------------------------------------------------------
void Player::moveExpo(uint32_t joyStickX, uint32_t joyStickY){

    //Change origin (and invert y) 
    int32_t x = joyStickX - 2048 ;
    int32_t y = joyStickY - 2048 ;
    int32_t signY = 1;
    int32_t signX = 1;
    if(y < 0){signY = -1;}
    if(x < 0){signX = -1;}

    // Square it 
    x = x * x;
    y = y * y;
    // Devide by big number
    x = x / 1048576;
    y = y / 1048576;
    // Fix Sign 
    x = x * signX;
    y = y * signY * -1; // invert y
    // Actually change the palyer's position 
    this->x += x;
    this->y += y;

}

//------------------------------------------------------------------------------
// Description: Returns the current X position of the player.
// Description: Returns the current Y position of the player.
//------------------------------------------------------------------------------
uint32_t Player::x_position(void){return this->x;}
uint32_t Player::y_position(void){return this->y;}
