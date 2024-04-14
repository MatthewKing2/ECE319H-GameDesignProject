/* SlidePot.cpp
 * Students put your names here
 * Modified: put the date here
 * 12-bit ADC input on ADC1 channel 5, PB18
 */
#include <ti/devices/msp/msp.h>
#include "../inc/Clock.h"
#include "../inc/SlidePot.h"
#define ADCVREF_VDDA 0x000
#define ADCVREF_INT  0x200




void SlidePot::Init(void){
// write code to initialize ADC1 channel 5, PB18
// Your measurement will be connected to PB18
// 12-bit mode, 0 to 3.3V, right justified
// software trigger, no averaging
  
  // So they give us some code  , but I have to change it to meet the following reqs:
    // 1) Channel 5
    // 2) 12-bit mode
    // 3) 0-3.3V
    // 4) Right justfied      -- no clue whats up w/ this one 
    // 5) Software Triggered
    // 6) No averaging        -- I think I handled this, not 100% sure tbh 

  // Program 7.3.1. Initialization of the ADC0 channel 6, PB20, using software start, internal reference, and busy-wait.
  ADC1->ULLMEM.GPRCM.RSTCTL = 0xB1000003; // 1) reset
  ADC1->ULLMEM.GPRCM.PWREN = 0x26000001;  // 2) activate
  Clock_Delay(24);                        // 3) wait
  ADC1->ULLMEM.GPRCM.CLKCFG = 0xA9000000; // 4) ULPCLK
  ADC1->ULLMEM.CLKFREQ = 7;               // 5) 40-48 MHz
  ADC1->ULLMEM.CTL0 = 0x00010000;         // 6) DONT DEVIDE (was divide by 8)
  ADC1->ULLMEM.CTL1 = 0x00000000;         // 7) mode
  ADC1->ULLMEM.CTL2 = 0x00000000;         // 8) MEMRES
  ADC1->ULLMEM.MEMCTL[0] = 5;             // 9) Channel 5 (was channel 6 is PB20)
  ADC1->ULLMEM.SCOMP0 = 0;                // 10) 8 sample clocks
  ADC1->ULLMEM.CPU_INT.IMASK = 0;         // 11) no interrupt
  
  // write this
}





uint32_t SlidePot::In(void){
  // write code to sample ADC1 channel 5, PB18 once
  // return digital result (0 to 4095)

  // No changes to be made to the code from textbook? 

  // Program 7.3.2. ADC sampling using software start and busy-wait. 
  ADC1->ULLMEM.CTL0 |= 0x00000001;             // 1) enable conversions
  ADC1->ULLMEM.CTL1 |= 0x00000100;             // 2) start ADC
  uint32_t volatile delay=ADC1->ULLMEM.STATUS; // 3) time to let ADC start
  while((ADC1->ULLMEM.STATUS&0x01)==0x01){}    // 4) wait for completion
  return ADC1->ULLMEM.MEMRES[0];               // 5) 12-bit result

}





// constructor, invoked on creation of class
// m and b are linear calibration coefficents
SlidePot::SlidePot(uint32_t m, uint32_t b){
  // write this, runs on creation
  this->slope = m;
  this->offset = b;
  
  // No, "this->" notation is not required, however it is objectivly more specific 
}




// Save ADC, and set the Semaphore
void SlidePot::Save(uint32_t n){
  // write this

  // Save ADC into a mailbox
  this->data = n; 
  // Set the Semaphore Flag
  this->flag = 1;
}





uint32_t SlidePot::Convert(uint32_t n){
	// write this
    // convert ADC raw sample n to fixed-point distance, 0.001cm
    // Impliment the formula derived through calibration
    // Position = ((1655*Data)>>12)-139
  //return ((1655*n)>>12)-139; 
  return ((1743*n)>>12)+82;
}


// ############################################################################
// Added my own Convert that has no argumnents and no return (better design imo)
void SlidePot::Convert2(){
	// convert ADC raw sample n to fixed-point distance, 0.001cm
  // Position = ((1655*Data)>>12)-139
  //this->distance = ((1655*this->data)>>12)-139; 
  this->distance = ((1743*this->data)>>12)+82;

}
// ############################################################################




// do not use this function
// it is added just to show you how SLOW floating point in on a Cortex M0+
float SlidePot::FloatConvert(uint32_t input){
  return 0.00048828125*input -0.0001812345;
}




void SlidePot::Sync(void){
  // write this
  while(this->flag == 0){} // wait for semaphore
  this->flag = 0;         // then clear semaphore
}





uint32_t SlidePot::Distance(void){  // return distance value (0 to 2000), 0.001cm
  // write this
  return this->distance; // replace this with solution
}

