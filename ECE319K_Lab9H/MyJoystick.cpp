
#include <ti/devices/msp/msp.h>
#include "MyJoystick.h"
#include "../inc/Clock.h"
#include "../inc/ADC.h"   // Using these ADC functions to set up and sample
#include <math.h>


Joystick::Joystick(void){
  this->x = 1;  // useless just for testing
  this->y = 1;
}


// Not using 
void Joystick::Joystick_Init(void){
 
  // //ADC12_Regs* adc12 = ADC1;
  // uint32_t channel1 = 1;   // PA16
  // uint32_t channel2 = 2;   // PA17
  // uint32_t reference = 0; // ADCVREF_VDDA; // this eqals 0

  // ADC1->ULLMEM.GPRCM.RSTCTL = (uint32_t)0xB1000003;    // 1) Reset 
  // ADC1->ULLMEM.GPRCM.PWREN = (uint32_t)0x26000001;     // 2) Activiate 
  // Clock_Delay(24);                                      // 3) Wait
  // ADC1->ULLMEM.GPRCM.CLKCFG = 0xA9000000;              // 4) ULPCLK
  // ADC1->ULLMEM.CLKFREQ = 7;                            // 5) 40 to 48 MHz
  // ADC1->ULLMEM.CTL0 = 0x00010000;                      // 6) No Devide (was 0x03010000)
  // ADC1->ULLMEM.CTL1 = 0x00010000;                      // 7) Mode, No Average (was 0x00010000)
  // ADC1->ULLMEM.CTL2 = 0x20100000;                      // 8) MEMRES (was 0x02010000)
  // ADC1->ULLMEM.MEMCTL[1] = 1;                          // 9) Channel 1 
  // ADC1->ULLMEM.MEMCTL[2] = 2;                          //    Channel 2
  // ADC1->ULLMEM.SCOMP0 = 0;                             // 10) 8 sample clocks
  // //ADC1->ULLMEM.GEN_EVENT.IMASK = 0;                    // 11) No interrrupt (slightly different than slidepot) 
  // ADC1->ULLMEM.CPU_INT.IMASK = 0;                    // 11) No interrrupt (slightly different than slidepot) 

  ADC_Init(ADC1,1,ADCVREF_VDDA); // x position joystick
  ADC_Init(ADC1,2,ADCVREF_VDDA); // y position joystick
  // assume these are called from LaunchPad_Init
  //  GPIOA->GPRCM.RSTCTL = (uint32_t)0xB1000003;  // Reset GPIOA
  //  GPIOA->GPRCM.PWREN = (uint32_t)0x26000001;   // Enable power to GPIOA
  Clock_Delay(24); // time for gpio to power up


}

// Not using 
void Joystick::Joystick_In(uint32_t* x, uint32_t* y){

  //ADC12_Regs* adc12 = ADC1;

  ADC1->ULLMEM.CTL0 |= 0x00000001;               // enable conversions
  ADC1->ULLMEM.CTL1 |= 0x00000100;               // start ADC
  uint32_t volatile delay=ADC1->ULLMEM.STATUS;   // time to let ADC start
  while((ADC1->ULLMEM.STATUS&0x01)==0x01){};     // wait for completion
  *x = ADC1->ULLMEM.MEMRES[1];                  // Channel 1
  *y = ADC1->ULLMEM.MEMRES[2];                  // Channel 2

  // ADC_InDual(ADC1, x, y);


}

// Am using 
// sample 12-bit ADC
void Joystick::ADC_InDual(ADC12_Regs *adc12,uint32_t *d1, uint32_t *d2){
  adc12->ULLMEM.CTL0 |= 0x00000001; // enable conversions
  adc12->ULLMEM.CTL1 |= 0x00000100; // start ADC
  uint32_t volatile delay=adc12->ULLMEM.STATUS; // time to let ADC start
  while((adc12->ULLMEM.STATUS&0x01)==0x01){}; // wait for completion
  *d1 = adc12->ULLMEM.MEMRES[1];
  *d2 = adc12->ULLMEM.MEMRES[2];
}


// Am using 
// Assumes 40 MHz bus
void Joystick::ADC_InitDual(ADC12_Regs *adc12,uint32_t channel1,uint32_t channel2, uint32_t reference){
    // Reset ADC and VREF
    // RSTCLR
    //   bits 31-24 unlock key 0xB1
    //   bit 1 is Clear reset sticky bit
    //   bit 0 is reset ADC
  adc12->ULLMEM.GPRCM.RSTCTL = (uint32_t)0xB1000003;
  if(reference == ADCVREF_INT){
    VREF->GPRCM.RSTCTL = (uint32_t)0xB1000003;
  }
    // Enable power ADC and VREF
    // PWREN
    //   bits 31-24 unlock key 0x26
    //   bit 0 is Enable Power
  adc12->ULLMEM.GPRCM.PWREN = (uint32_t)0x26000001;
  if(reference == ADCVREF_INT){
    VREF->GPRCM.PWREN = (uint32_t)0x26000001;
  }
  Clock_Delay(24); // time for ADC and VREF to power up
  adc12->ULLMEM.GPRCM.CLKCFG = 0xA9000000; // ULPCLK
  // bits 31-24 key=0xA9
  // bit 5 CCONSTOP= 0 not continuous clock in stop mode
  // bit 4 CCORUN= 0 not continuous clock in run mode
  // bit 1-0 0=ULPCLK,1=SYSOSC,2=HFCLK
  adc12->ULLMEM.CLKFREQ = 7; // 40 to 48 MHz
  adc12->ULLMEM.CTL0 = 0x03010000;
  // bits 26-24 = 011 divide by 8
  // bit 16 PWRDN=1 for manual, =0 power down on completion, if no pending trigger
  // bit 0 ENC=0 disable (1 to 0 will end conversion)
  adc12->ULLMEM.CTL1 = 0x00010000;
  // bits 30-28 =0  no shift
  // bits 26-24 =0  no averaging
  // bit 20 SAMPMODE=0 high phase
  // bits 17-16 CONSEQ=01 ADC at start to end
  // bit 8 SC=0 for stop, =1 to software start
  // bit 0 TRIGSRC=0 software trigger
  adc12->ULLMEM.CTL2 = 0x02010000;
  // bits 28-24 ENDADD=2 (which  MEMCTL to end)
  // bits 20-16 STARTADD=1 (which  MEMCTL to start)
  // bits 15-11 SAMPCNT (for DMA)
  // bit 10 FIFOEN=0 disable FIFO
  // bit 8  DMAEN=0 disable DMA
  // bits 2-1 RES=0 for 12 bit (=1 for 10bit,=2for 8-bit)
  // bit 0 DF=0 unsigned formant (1 for signed, left aligned)
  adc12->ULLMEM.MEMCTL[1] = reference+channel1;
  adc12->ULLMEM.MEMCTL[2] = reference+channel2;
  // bit 28 WINCOMP=0 disable window comparator
  // bit 24 TRIG trigger policy, =0 for auto next, =1 for next requires trigger
  // bit 20 BCSEN=0 disable burn out current
  // bit 16 = AVGEN =0 for no averaging
  // bit 12 = STIME=0 for SCOMP0
  // bits 9-8 VRSEL = 10 for internal VREF,(00 for VDDA)
  // bits 4-0 channel = 0 to 7 available
  adc12->ULLMEM.SCOMP0 = 0; // 8 sample clocks
//  adc12->ULLMEM.GEN_EVENT.ICLR |= 0x0100; // clear flag MEMCTL[1] ??
  adc12->ULLMEM.GEN_EVENT.IMASK = 0; // no interrupt
  if(reference == ADCVREF_INT){
    VREF->CLKSEL = 0x00000008; // bus clock
    VREF->CLKDIV = 0; // divide by 1
    VREF->CTL0 = 0x0001;
  // bit 8 SHMODE = off
  // bit 7 BUFCONFIG=0 for 2.4 (=1 for 1.4)
  // bit 0 is enable
    VREF->CTL2 = 0;
  // bits 31-16 HCYCLE=0
    // bits 15-0 SHCYCLE=0
    while((VREF->CTL1&0x01)==0){}; // wait for VREF to be ready
  }
}

// Am using 
uint32_t Joystick::Joystick_InButton(void){
  return GPIOB->DIN31_0 & (1<<16);  // pb16
}


