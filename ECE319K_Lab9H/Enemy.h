
#ifndef ENEMY_H_
#define ENEMY_H_
#include <stdint.h>
#include "Frame.h"

class Enemy{ 
  public: // Data 
    int32_t x;         // Player X and Y position 
    int32_t y;
    uint16_t h,w;       // Player Width and Hight 
    uint32_t color;     // Player color
    const uint16_t* image;    // What player looks like
    bool alive;         // Is player alive
    uint32_t frame;     // What frame enemy is in

    // Mailbox system
    uint32_t prevFrame; 
    int32_t prevX;      
    int32_t prevY;
    bool changedFrames; 
        // If the enemy changed frames, and their previsou frame was the frame I am in
        // then i must erase them b/c they are nolonger here. To do this i use their 
        // previsou X and Y (from before their leaving of my frame) 

  public: // Functions (in the order they would be called)
    Enemy(int32_t x, int32_t y, const uint16_t* image, uint32_t frame); // Testing Constructor 
    Enemy();                                                            // Default Constructor
    void updatePosition(int32_t x, int32_t y, uint32_t frame, bool alive); // Updates enemy object
    void assignSprite(const uint16_t* image);                           // Update Enemies Sprite
};
#endif



