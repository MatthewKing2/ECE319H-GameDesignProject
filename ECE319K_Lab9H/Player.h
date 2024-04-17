
// SlidePot.h
// Runs on MSPM0
// Provide functions that initialize ADC1 channel 5, PB18 and use a slide pot to measure distance
// Created: 12/29/2023
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly


#ifndef PLAYER_H_
#define PLAYER_H_
#include <stdint.h>

class Player{ 
  private: // Data 
    uint32_t x;         // Player X and Y position 
    uint32_t y;
    uint32_t frame;     // Frame of map player is on
    uint16_t h,w;       // Player Width and Hight 
    uint32_t color;     // Player color
    bool alive;         // Is player alive
    bool murder;        // Is the player the murderur 

  public: // Functions (in the order they would be called)
    Player(uint32_t m, uint32_t b, uint32_t frame, bool murder);      // Constructor 
    void move(int32_t joyStickX, int32_t joyStickY);                  // Move based on input
    void moveLinear(uint32_t joystickX, uint32_t joystickY);          // Linear movment
    void moveExpo(uint32_t joystickX, uint32_t joystickY);            // Exponential movment? 
    uint32_t x_position(void);          // X position (getter) 
    uint32_t y_position(void);          // Y position (getter) 

};
#endif


