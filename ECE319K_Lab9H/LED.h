/*
 * LED.h
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */

#ifndef LED_H_
#define LED_H_

void redOn(void);
void redOff(void);
void greenOn(void);
void greenOff(void);

// initialize your LEDs
void LED_Init(void);

// data specifies which LED to toggle
void LED_Toggle(uint32_t data);

#endif /* LED_H_ */
