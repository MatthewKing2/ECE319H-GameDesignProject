
//------------------------------------------------------------------------------
// File: MyButtons.cpp
// Description: Implements the functionality for handling button presses. 
//              This includes initializing the button inputs, reading 
//              their states, and providing methods to check if buttons are 
//              pressed. 
// Dependencies: LaunchPad.h
//------------------------------------------------------------------------------

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "MyButtons.h"

// Default Constructor
Buttons::Buttons(){
    return; 
}

// Initlize Buttons 
void Buttons::Buttons_Init(void){
    IOMUX->SECCFG.PINCM[PB24INDEX] = 0x00050081; //UP -> x001
    IOMUX->SECCFG.PINCM[PB20INDEX] = 0x00050081; //DOWN -> x004
    IOMUX->SECCFG.PINCM[PB19INDEX] = 0x00050081; //LEFT -> x002
    IOMUX->SECCFG.PINCM[PB18INDEX] = 0x00050081; //RIGHT -> x008
    IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00050081; //DASH -> x010
    return;
} 

// Read Button Inputs
void Buttons::Buttons_In(uint32_t* up, uint32_t* down, uint32_t* left, uint32_t* right, uint32_t* dash){
    up[0]   = (GPIOB->DIN31_0 & (1<<24))>>24; // Shift to correct bit values
    down[0] = (GPIOB->DIN31_0 & (1<<20))>>20; 
    left[0] = (GPIOB->DIN31_0 & (1<<19))>>19;
    right[0]= (GPIOB->DIN31_0 & (1<<18))>>18;
    dash[0] = (GPIOA->DIN31_0 & (1<<27))>>27;
    return;
}

// Buttons In, but with delay to prevent button spamming 
void Buttons::Menu_Buttons_In(uint32_t* up, uint32_t* down, uint32_t* left, uint32_t* right, uint32_t* dash){
    static uint32_t count = 100000;
    if(count == 0){
        Buttons_In(up, down, left, right, dash);
        count = 100000;
    }else{
        count --;
    }
}

// See if button as been pressed
bool Buttons::Buttons_Pressed(){
    int a = GPIOB->DIN31_0 & 0x1000000;
    if( (GPIOB->DIN31_0 & 0x1000000)  || (GPIOB->DIN31_0 & 0x100000) || (GPIOB->DIN31_0 & 0x80000) ||  (GPIOB->DIN31_0 & 0x40000) ||  (GPIOA->DIN31_0 & 0x8000000) ){
        return true;
    }
    return false;
}
