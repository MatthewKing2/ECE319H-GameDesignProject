
//------------------------------------------------------------------------------
// File: LED.h
// Description: Functions protypes for Red and Green LEDs.
//------------------------------------------------------------------------------

#ifndef LED_H_
#define LED_H_

void redOn(void);
void redOff(void);
void greenOn(void);
void greenOff(void);
void LED_Init(void);
void LED_Toggle(uint32_t data);

#endif /* LED_H_ */