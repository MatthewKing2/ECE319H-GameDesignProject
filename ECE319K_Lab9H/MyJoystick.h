
//------------------------------------------------------------------------------
// File: MyJoystick.cpp
// Description: Function Protypes for interfacing with a joystick.
//------------------------------------------------------------------------------

#ifndef MYJOYSTICK_H_
#define MYJOYSTICK_H_
#include <stdint.h>


class Joystick{ 
  private: 
    uint32_t x;         // Player X and Y position 
    uint32_t y;

  public: 
    Joystick(void);                             
    void ADC_InitDual(ADC12_Regs *adc12,uint32_t channel1,uint32_t channel2, uint32_t reference);
    void ADC_InDual(ADC12_Regs *adc12,uint32_t *d1, uint32_t *d2);
    uint32_t Joystick_InButton(void);           // Return joystick button press

    // Not using 
    void Joystick_Init(void);                   // Init the joystick
    void Joystick_In(uint32_t* x, uint32_t* y); // "Return" X and Y values from ADC

};

#endif
