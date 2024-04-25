/*
 * LED.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
// LaunchPad.h defines all the indices into the PINCM table

// initialize your LEDs
void LED_Init(void){
    IOMUX->SECCFG.PINCM[PB12INDEX] = 0x00000081; // regular output
    IOMUX->SECCFG.PINCM[PB13INDEX] = 0x00000081; // regular output

    GPIOB->DOE31_0 |= 0x3000; // enable
}

void greenOn(void){
    //GPIOB->DOUT31_0 |= 0x2000;
    GPIOB->DOUTSET31_0 |= 0x2000;
}

void greenOff(void){
    //GPIOB->DOUT31_0 &= ~0x2000;
    GPIOB->DOUTCLR31_0 |= 0x2000;
}

void redOn(void){
    //GPIOB->DOUT31_0 |= 0x1000;
    GPIOB->DOUTSET31_0 |= 0x1000;
}

void redOff(void){
    //GPIOB->DOUT31_0 &= ~0x1000;
    GPIOB->DOUTCLR31_0 |= 0x1000;
}

// data specifies which LED to toggle
void LED_Toggle(uint32_t data){
    // write this
    // use DOUTTGL31_0 register so it does not interfere with other GPIO
 
}
