
//------------------------------------------------------------------------------
// File: Enemy.
// Author: Matthew King
// Description: Implementation file for the Enemy class. This file contains the
//              definitions of the Enemy class's member functions, handling 
//              the initialization, updating of position, and sprite management.
// Dependencies: Enemy.h, images.h
//------------------------------------------------------------------------------

#include <ti/devices/msp/msp.h>
#include "Enemy.h"
#include "../inc/ST7735.h"
#include "images/images.h"

//------------------------------------------------------------------------------
// Function: Enemy::assignSprite
// Description: Sets a new sprite image for the enemy. This function simply
//              assigns the provided image pointer to the object's image member.
//------------------------------------------------------------------------------
void Enemy::assignSprite(const uint16_t* image){
    this->image = image;
}

//------------------------------------------------------------------------------
// Function: Enemy::Enemy(int32_t x, int32_t y, const uint16_t* image, uint32_t frame)
// Description: Parameterized constructor initializes the enemy's position, 
//              sprite, and state. Sets default values for height, width, and color.
//              Noteworthy:
//              - Initializes default dimensions (16x16) and color (blue).
//------------------------------------------------------------------------------
Enemy::Enemy(int32_t x, int32_t y, const uint16_t* image, uint32_t frame){
  this->x = x;
  this->y = y;
  this->prevX = 0;
  this->prevX = 0;
  this->h = 16;
  this->w = 16;
  this->color = ST7735_Color565(0, 0, 255);
  this->image = image;
  this->alive = true;
  this->frame = frame;          
}

//------------------------------------------------------------------------------
// Function: Enemy::Enemy()
// Description: Default constructor initializes the enemy with a default position
//              off-screen, a default image, and marks it as alive. Sets frame to
//              an out-of-map value.
//              Noteworthy:
//              - Default image and color are set.
//              - Puts the enemy off-screen by default (should never be called)
//------------------------------------------------------------------------------
Enemy::Enemy(){
  this->x = 255;
  this->y = 255;
  this->prevX = 0;
  this->prevX = 0;
  this->h = 16;
  this->w = 16;
  this->color = ST7735_Color565(0, 0, 255);
  this->image = bobaFett;
  this->alive = true;
  this->frame = 10;
}

//------------------------------------------------------------------------------
// Function: Enemy::updatePosition
// Description: Updates the enemy's position, frame, and state. Also handles
//              checking if the enemy has changed frames or moved significantly
//              (jitter prevention).
//              Noteworthy:
//              - Includes error checking to ensure position data is within 
//                valid bounds (6 to 114).
//              - Detects if the enemy has moved enough to consider it a 
//                "frame change", if so the a flag is set, so the enemy gets
//                erased (so the sprite does not linger in a frame its left).
//              - Also detects if the enemy has "jittered" (moved more than 
//                possible). This means that a packet from the other controller
//                was missed, so we have to manually erase where the enemy was.
//------------------------------------------------------------------------------
void Enemy::updatePosition(int32_t x, int32_t y, uint32_t frame, bool alive){
    // Error checking to ensure data is valid (prevent bug where enemy glitches to top of screen)
    if(x < 6 || x > 114){return;}
    if(y < 6 || y > 114){return;}

    this->alive = alive;
    this->prevX = this->x;
    this->prevY = this->y;
    this->x = x;
    this->y = y;
    if(this->frame != frame){
        this->changedFrames = true;
        this->prevFrame = this->frame;
    }else{
        this->changedFrames = false;
    }
    this->frame = frame;

    // "Jitter" checking on enemy player movment
    if((this->x < (this->prevX-4))||(this->x > (this->prevX+4))){
      this->changedFrames = true;
    }
    if((this->y < (this->prevY-4))||(this->y > (this->prevY+4))){
      this->changedFrames = true;
    }
}