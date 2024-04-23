/* SlidePot.cpp
 * Students put your names here
 * Modified: put the date here
 * 12-bit ADC input on ADC1 channel 5, PB18
 */
#include <ti/devices/msp/msp.h>
#include "Player.h"
#include "../inc/ST7735.h"
#include <math.h>
#include "LaserShot.h"
#define playerSize 16 
#define PI 3.14159265
extern Frame frames[];
extern Shot shots[];


// constructor, invoked on creation of class
Player::Player(int32_t x, int32_t y, const uint16_t* image, bool murder){
  // write this, runs on creation
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

// Goes through the awrray of shots and returns true if a valid enemy shot is touching the player
bool Player::touchingLaser(uint32_t currFrameIndex){
    bool touching = false;
    for(int i = 1; i < 4; i++){ // Index 0 is my shot, index 1,2,3 are enemy shots
        if(shots[i].valid){ // Dont care about old shots (invalid ones)
            touching |= shots[i].touching(this->x,this->y,8,8);
        }
    }
    return touching;
}



// Uses global Frames array to determine if touching an exit
// Returns T/F if touching, and the frameindex to new frame
// Updates player X and Y attributes to new position 
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


// Computes the new X and Y using the joystick input 
// Assuming: JoystickX and Y are in the range: [0, 4096]
// Note:
    // If testing, call with negative Y to go down 
    // call with positive Y to go up 
    // This funciton handles the conversion 
void Player::move(int32_t joyStickX, int32_t joyStickY){

    //Determine if in deadzone: 50% center (dont move)
    if(1024 < joyStickX && joyStickX < 3072 && 1024 < joyStickY && joyStickY < 3072){
        return;
    }

    //Figure out the direction (and remove sign)
    double x = joyStickX - 2048 ;
    double y = joyStickY - 2048 ;

    // double x = joyStickX;
    // double y = joyStickY; 

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


void Player::reallyBadMaxMove(int32_t deltaX, int32_t deltaY, uint32_t currFrameIndex){

    int32_t Xover = 0;
    int32_t Yover = 0;

    int32_t maxX = 0;
    int32_t maxY = 0;

    // Negative X 
    if(deltaX < 0){
        bool touchingX = false;
        while((maxX >= deltaX)&&(!touchingX)){
            for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
                touchingX = frames[currFrameIndex].walls[i].touching(this->x + deltaX -4, this->y + deltaY -4, 16, 16, &Xover, &Yover);
            }
            maxX --;
        }
        maxX ++;
    }
    // Positive X
    else{
        bool touchingX = false;
        while((maxX <= deltaX)&&(!touchingX)){
            for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
                touchingX = frames[currFrameIndex].walls[i].touching(this->x + deltaX -4, this->y + deltaY -4, 16, 16, &Xover, &Yover);
            }
            maxX ++;
        }
        maxX --;
    }

    // Negative Y
    if(deltaY < 0){
        bool touchingY = false;
        while((maxY >= deltaY)&&(!touchingY)){
            for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
                touchingY = frames[currFrameIndex].walls[i].touching(this->x + deltaX -4, this->y + deltaY -4, 16, 16, &Xover, &Yover);
            }
            maxY --;
        }
        maxY ++;
    }
    // Positive X
    else{
        bool touchingY = false;
        while((maxY <= deltaY)&&(!touchingY)){
            for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
                touchingY = frames[currFrameIndex].walls[i].touching(this->x + deltaX -4, this->y + deltaY -4, 16, 16, &Xover, &Yover);
            }
            maxY ++;
        }
        maxY --;
    }


    this->x += maxX; 
    this->y += maxY; 


}


// Depending on the walls in the current frame, and the player's desired movement
// Move them the maximum amount (until touching wall)
void Player::maxMove(int32_t deltaX, int32_t deltaY, uint32_t currFrameIndex){

    int32_t Xover = 0;
    int32_t Yover = 0;

    for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
        frames[currFrameIndex].walls[i].touching(this->x + deltaX -4, this->y + deltaY -4, 16, 16, &Xover, &Yover);
    }

    // B/C Xover & Yover are simply how far to get out of shape, when added with deltaX,Y value may be greater than 4
    // Therefore, need to round down to 4 (but mod 5 is only for positive numbers), therefore this solution:
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


void Player::moveLinear(uint32_t joyStickX, uint32_t joyStickY, uint32_t currFrameIndex){

    //Change origin (and invert y) 
    int32_t x = joyStickX - 2048 ;
    int32_t y = joyStickY - 2048 ;
    y *= -1;
    x *= -1;    // x is also inverted apparently

    // Max in each direction is 2048 
    // Want 4 pixles of movement ==> 2048 / (2048/4)
        // ==> 512 
    x /= 512;
    y /= 512;
    
    // See if hitting any walls 
    // ##################################################
    // Wall Stuff
        // See if touching a wall 
        // Seperate into X and Y detection
    // bool touchingX = false;
    // for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
    //     touchingX |= frames[currFrameIndex].walls[i].touching(this->x + x -4, this->y-4, 16, 16);
    // }

    // bool touchingY = false;
    // for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
    //     touchingY |= frames[currFrameIndex].walls[i].touching(this->x -4, this->y + y -4, 16, 16);
    // }
    // // If not touching, change the palyers position
    // if(!touchingX){
    //     this->x += x;
    // }
    // if(!touchingY){
    //     this->y += y;
    // }

    maxMove(x, y, currFrameIndex);
  

}


// void Player::errorCorrect();    // Makes any out of range X and Y in range

void Player::moveExpo(uint32_t joyStickX, uint32_t joyStickY){

    //Change origin (and invert y) 
    int32_t x = joyStickX - 2048 ;
    int32_t y = joyStickY - 2048 ;
    int32_t signY = 1;
    int32_t signX = 1;

    if(y < 0){
        signY = -1;
    }

    if(x < 0){
        signX = -1;
    }

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


// Position Getter
uint32_t Player::x_position(void){
    return this->x;
}



// Position Getter
uint32_t Player::y_position(void){
    return this->y;
}
