
//------------------------------------------------------------------------------
// File: MyButtons.cpp
// Description: Function Protypes for handling button presses. 
//------------------------------------------------------------------------------

#ifndef MYBUTTONS_H_
#define MYBUTTONS_H_
#include <stdint.h>

class Buttons{ 
  public: 
    Buttons();
    void Buttons_Init(void);
    void Buttons_In(uint32_t* up, uint32_t* down, uint32_t* left, uint32_t* right, uint32_t* dash); 
    bool Buttons_Pressed();
    void Menu_Buttons_In(uint32_t* up, uint32_t* down, uint32_t* left, uint32_t* right, uint32_t* dash); 
};

#endif
