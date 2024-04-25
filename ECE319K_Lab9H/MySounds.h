/*
 * MySounds.h
 *
 *  Created on: Apr 21, 2024
 *      Author: simon
 */

#ifndef MYSOUNDS_H_
#define MYSOUNDS_H_

void Sound_Init(void);
void SysTick_Handler(void);
void Sound_Start(void);
void Sound_Stop(void);


#endif /* MYSOUNDS_H_ */

//MAKE SURE TO INIT DAC AT BEGINNING OF MAIN PROGRAM
//  DAC5_Init();
//Sound_Init(1,0);







