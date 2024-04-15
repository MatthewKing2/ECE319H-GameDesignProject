/* SlidePot.cpp
 * Students put your names here
 * Modified: put the date here
 * 12-bit ADC input on ADC1 channel 5, PB18
 */
#include <ti/devices/msp/msp.h>
#include "Player.h"
#include <math.h>
#define playerSize 8 
#define PI 3.14159265

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

// Computes the new X and Y using the joystick input 
// Assuming: JoystickX and Y are in the range: [0, 4096]
// Note:
    // If testing, call with negative Y to go down 
    // call with positive Y to go up 
    // This funciton handles the conversion 
void Player::move(int32_t joyStickX, int32_t joyStickY){

    // Determine if in deadzone: 50% center (dont move)
    // if(1024 < joyStickX && joyStickX < 3072 && 1024 < joyStickY && joyStickY < 3072){
    //     return;
    // }

    // Figure out the direction (and remove sign)
    // int32_t x = joyStickX - 2048 ;
    // int32_t y = joyStickY - 2048 ;

    double x = joyStickX;
    double y = joyStickY; 

    int8_t vertSign = -1;   // Default UP (Inverse of what you'd think b/c LCD has origin @ top left)
    int8_t horzSign = 1;    // Defulat Right
    if(y < 0){
        vertSign = 1;       // Go Down (increase on y axis)
        y *= -1;
    }
    if(x < 0){
        horzSign = -1;      // Go Left
        x *= -1;
    }

    // Compute the Angle  
    double angle;
    if(x == 0 && y == 0){   // wont need this when implimented deadzone
        return;
    }
    else{
        // Need proportion of X and Y 
        double sum = x + y;
        x = x / sum; 
        y = y / sum; 
        angle = atan2 (y, x) * (180.0 / PI);
            // atan2 takes into acount 0 values 
    }

    // Use angle to dictate movment (first quadrent)
    int8_t deltaX = 0;
    int8_t deltaY = 0;
    // Right? 
    if(angle <= 11.25){
       deltaX = 2*horzSign;
    }
    // Mostly Right?
    else if(angle <= 33.75){
        deltaX = 2*horzSign;
        deltaY = 1*vertSign;
    }
    // Diangle?
    else if(angle <= 56.25){
        deltaX = 2*horzSign;
        deltaY = 2*vertSign;
    }
    // Mostly up?
    else if(angle <= 78.75){
        deltaX = 1*horzSign;
        deltaY = 2*vertSign;
    }
    // Up? 
    else{
       deltaY = 2*vertSign;
    }

    // Actually change the palyer's position 
    this->x += deltaX;
    this->y += deltaY;

}

// Position Getter
uint32_t Player::x_position(void){
    return this->x;
}

// Position Getter
uint32_t Player::y_position(void){
    return this->y;
}
