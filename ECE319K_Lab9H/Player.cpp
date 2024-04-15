/* SlidePot.cpp
 * Students put your names here
 * Modified: put the date here
 * 12-bit ADC input on ADC1 channel 5, PB18
 */
#include <ti/devices/msp/msp.h>
#include "Player.h"
#define playerSize 8 

// constructor, invoked on creation of class
// m and b are linear calibration coefficents
Player::Player(uint32_t x, uint32_t y, uint32_t frame, bool murder){
  // write this, runs on creation
  this->x = x;
  this->y = y;
  this->frame = 0;
  this->h = playerSize;
  this->w = playerSize;
  this->color = 255;
  this->murder = false;
  this->alive = true;
}

// TODO
// Computes the new X and Y using the joystick input 
void Player::move(int32_t joyStickX, int32_t joyStickY){
    this->x += joyStickX;
    this->y += joyStickY;
}

// Position Getter
uint32_t Player::x_position(void){
    return this->x;
}

// Position Getter
uint32_t Player::y_position(void){
    return this->y;
}
