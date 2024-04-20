
// SlidePot.h
// Runs on MSPM0
// Provide functions that initialize ADC1 channel 5, PB18 and use a slide pot to measure distance
// Created: 12/29/2023
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly


#ifndef PLAYER_H_
#define PLAYER_H_
#include <stdint.h>
#include "Frame.h"

class Player{ 
  public: // Data 
    uint32_t x;         // Player X and Y position 
    uint32_t y;
    uint16_t h,w;       // Player Width and Hight 
    uint32_t color;     // Player color
    bool alive;         // Is player alive
    bool murder;        // Is the player the murderur 

  public: // Functions (in the order they would be called)
    Player(uint32_t m, uint32_t b, bool murder);          // Constructor 
    // Movment Functions
      void move(int32_t joyStickX, int32_t joyStickY);
      void moveLinear(uint32_t joystickX, uint32_t joystickY, uint32_t frameIndex); 
      void moveExpo(uint32_t joystickX, uint32_t joystickY);              
      void errorCorrect();    // Makes any out of range X and Y in range
    // Colision Functions
      bool touchingExit(uint32_t currFrameIndex, uint32_t* newFrameIndex); // Player touching exit 
      void touchingWall(uint32_t x, uint32_t y, bool* touchingX, bool* touchingY);
    // Getters
    uint32_t x_position(void);          // X position (getter) 
    uint32_t y_position(void);          // Y position (getter) 

};
#endif


