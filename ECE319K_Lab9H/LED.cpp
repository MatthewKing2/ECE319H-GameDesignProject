
//------------------------------------------------------------------------------
// File: LED.cpp
// Description: Contains functions to initialize and control LEDs connected to 
//              the LaunchPad. Provides functionality to turn the Red and Green
//              LEDs on and off. Uses GPIO registers to control the LEDs' 
//              states based on input data.
// Dependencies: LaunchPad.h  (defines all the indices into the PINCM table)
//------------------------------------------------------------------------------

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"

// initialize your LEDs
void LED_Init(void){
    IOMUX->SECCFG.PINCM[PB12INDEX] = 0x00000081; // regular output
    IOMUX->SECCFG.PINCM[PB13INDEX] = 0x00000081; // regular output
    GPIOB->DOE31_0 |= 0x3000; // enable
}

void greenOn(void){
    GPIOB->DOUTSET31_0 |= 0x2000;
}

void greenOff(void){
    GPIOB->DOUTCLR31_0 |= 0x2000;
}

void redOn(void){
    GPIOB->DOUTSET31_0 |= 0x1000;
}

void redOff(void){
    GPIOB->DOUTCLR31_0 |= 0x1000;
}

void LED_Toggle(uint32_t data){
}
