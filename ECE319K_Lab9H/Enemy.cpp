

#include <ti/devices/msp/msp.h>
#include "Enemy.h"
#include "../inc/ST7735.h"
#include "images/images.h"

// class Enemy{ 
//   public: // Data 
//     int32_t x;         // Player X and Y position 
//     int32_t y;
//     int32_t prevX;         // Player X and Y position 
//     int32_t prevY;
//     uint16_t h,w;       // Player Width and Hight 
//     uint32_t color;     // Player color
//     const uint16_t* image;    // What player looks like
//     bool alive;         // Is player alive
//     uint32_t frame;     // What frame enemy is in

//   public: // Functions (in the order they would be called)
//     Enemy(int32_t x, int32_t y, const uint16_t* image);         // Testing Constructor 
//     Enemy();                                                    // Default Constructor
//     void updatePosition(int32_t x, int32_t y, uint32_t frame);  // Updates enemy object

// };



// Update Enemies Sprite
void Enemy::assignSprite(const uint16_t* image){
    this->image = image;
}


// constructor, invoked on creation of class
Enemy::Enemy(int32_t x, int32_t y, const uint16_t* image, uint32_t frame){
  // write this, runs on creation
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

Enemy::Enemy(){
  // write this, runs on creation
  this->x = 255;    // Image is totally off the screen, wont display
  this->y = 255;
  this->prevX = 0;
  this->prevX = 0;
  this->h = 16;
  this->w = 16;
  this->color = ST7735_Color565(0, 0, 255);
  this->image = baseEnemy8x8;
  this->alive = true;
  this->frame = 10;          // Out of Map by default 
}

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

    // Error checking on enemy player movment
      // b/c some packets are lost, enemy can jitter
    if((this->x < (this->prevX-4))||(this->x > (this->prevX+4))){
      this->changedFrames = true;
    }
    if((this->y < (this->prevY-4))||(this->y > (this->prevY+4))){
      this->changedFrames = true;
    }
}