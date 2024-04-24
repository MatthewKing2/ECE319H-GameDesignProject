#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "MyButtons.h"

// class Buttons{ 
//   private: // Data 
//     // None rn

//   public: // Functions 
//     Buttons();                  // Constructor (initlizes the buttons)
//     void Buttons_Init(void);    // Init the joystick (not going to use if I do it in the constructor)
//     void Buttons_In(uint32_t* up, uint32_t* down, uint32_t* left, uint32_t* right, uint32_t* dash); 
//                                 // Return button values for each button(either 0 or 1)

// };

// #define PB23INDEX 50
// #define PB24INDEX 51
// #define PB19INDEX 44
// #define PA27INDEX 30
// #define PB18INDEX 43

Buttons::Buttons(){
    // Do nothing 
    return;
}

void Buttons::Buttons_Init(void){
    // TODO
    IOMUX->SECCFG.PINCM[PB24INDEX] = 0x00050081; //UP -> x001
    IOMUX->SECCFG.PINCM[PB20INDEX] = 0x00050081; //DOWN -> x004
    IOMUX->SECCFG.PINCM[PB19INDEX] = 0x00050081; //LEFT -> x002
    IOMUX->SECCFG.PINCM[PB18INDEX] = 0x00050081; //RIGHT -> x008
    IOMUX->SECCFG.PINCM[PA27INDEX] = 0x00050081; //DASH -> x010
    return;
} 

void Buttons::Buttons_In(uint32_t* up, uint32_t* down, uint32_t* left, uint32_t* right, uint32_t* dash){


    // Read in the values and return them (using pointers)
    up[0]   = (GPIOB->DIN31_0 & (1<<24))>>24; // Have to shift to the right so they're not wrong big values 
    down[0] = (GPIOB->DIN31_0 & (1<<20))>>20; 
    left[0] = (GPIOB->DIN31_0 & (1<<19))>>19;
    right[0]= (GPIOB->DIN31_0 & (1<<18))>>18;
    dash[0] = (GPIOA->DIN31_0 & (1<<27))>>27;

    return;
}