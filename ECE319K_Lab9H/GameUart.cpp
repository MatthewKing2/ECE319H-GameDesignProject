
#include <ti/devices/msp/msp.h>
#include "GameUart.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
//#include "GameFifo.h"
#include "../inc/FIFO2.h"

uint32_t LostData = 0;
Queue FIFO1;
Queue FIFO2;
Queue FIFO3;
char data[4];


// Receiver Class
// ############################################################################################
// ############################################################################################
// ############################################################################################
// ############################################################################################
// ############################################################################################
// ############################################################################################

Receiver::Receiver(){
    this->frame = 10; // Out of Frame be fault 
}

void Receiver::receiverTranslate(char one, char two, char three, char four){
    this->msg = one&0x7F;
    this->frame = (two&(3<<5))>>5;  // Need to shift it back b/c its not at the start of byte like direction
    this->alive = two&(1<<4);
    this->pickup = two&(1<<3);
    this->shot = two&(1<<2);
    this->shotDirection = two&(3);  // 2 bits ==> &3 
    this->x = three&0x7F;
    this->y = four&0x7F;
}




// UART2
// ############################################################################################
// ############################################################################################
// ############################################################################################
// ############################################################################################
// ############################################################################################
// ############################################################################################

// for 80MHz bus clock, UART2 clock is ULPCLK 40MHz
// initialize UART2 for 115200 baud rate
// no transmit, interrupt on receive timeout
void UART2_Init(void){
  UART2->GPRCM.RSTCTL = 0xB1000003;               // reset UART0
  UART2->GPRCM.PWREN = 0x26000001;                // activate UART0
  Clock_Delay(24);                                // time for uart to activate
  //IOMUX->SECCFG.PINCM[PB17INDEX] = 0x00000082;    // Regular Output PB17 = Tx
  IOMUX->SECCFG.PINCM[PA24INDEX] = 0x00040082;    // Regular Input PA24 = Rx
  UART2->CLKSEL = 0x08;               // bus clock
  UART2->CLKDIV = 0x00;               // no divide
  UART2->CTL0 &= ~0x09;               // was 1, disable uart
  UART2->CTL0 = 0x00020018;           // enable fifos, tx and rx
  UART2->IBRD = 21;                   // From Textbook 115200
  UART2->FBRD = 45;                     
//   UART2->IBRD = 1052;                   // divider = 1052+(40/64) = 1052.625
//   UART2->FBRD = 40;
  UART2->LCRH = 0x00000030;           // 8bit, 1 stop, no parity
  UART2->CPU_INT.IMASK = 0x0001;        // JUST RTOUT (see pg. 286 for TXINT and RXINT, bits 11,10 respectivly)
    // Change to bit 10 x401
  UART2->IFLS = 0x422;
  NVIC->ICPR[0] = 1<<14;               // NO IDEAD WHAT THESE DO
  NVIC->ISER[0] = 1<<14;                // Different for different UARTs
  NVIC->IP[3] = (NVIC->IP[3]& (~0x00FF0000))|(2<<22);   // Sets priority 
  UART2->CTL0 |= 0x09;                  // was 1 - enable uart

    // // UART2 Initialization
    // UART2->GPRCM.RSTCTL = 0xB1000003;
    // UART2->GPRCM.PWREN = 0x26000001;
    // Clock_Delay(24); // time for uart to power up
    // IOMUX->SECCFG.PINCM[PA24INDEX]  = 0x00040082;
    // UART2->CLKSEL = 0x08; // bus clock
    // UART2->CLKDIV = 0x00; // no divide
    // UART2->CTL0 &= ~0x01; // disable UART2
    // UART2->CTL0 = 0x00020018;
    // // assumes an 80 MHz bus clock
    // UART2->IBRD = 800;//   divider = 800 + 0/64 = 800
    // UART2->FBRD = 0; // baud =2,500,000/800 = 3125
    // UART2->LCRH = 0x00000030;
    // UART2->CPU_INT.IMASK = 1;
    // // bit 11 TXINT
    // // bit 10 RXINT             Interrupt only on receiver timeout RTOUT (no TXINT, no RXINT)
    // // bit 0  Receive timeout
    // UART2->IFLS = 0x0422;
    // // bits 11-8 RXTOSEL receiver timeout select 4 (0xF highest)
    // // bits 6-4  RXIFLSEL 2 is greater than or equal to half
    // // bits 2-0  TXIFLSEL 2 is less than or equal to half
    // NVIC->ICPR[0] = 1<<14; // UART2 is IRQ 14
    // NVIC->ISER[0] = 1<<14;
    // NVIC->IP[3] = (NVIC->IP[3]&(~0xFF000000))|(2<<22);    // priority (bits 23,22)
    // UART2->CTL0 |= 0x01; // enable UART24


}



//------------UART2_InChar------------
// Get new serial port receive data from FIFO2
// Input: none
// Output: Return 0 if the FIFO2 is empty
//         Return nonzero data from the FIFO1 if available
char UART2_InChar(void){char out;
  // write this
  char data;
  bool valid = FIFO2.Get(&data);
  // If the data is valid, return the valid
  if(valid){return data;}
  // Otherwise, return 0 
  return 0;
}

void UART2_InData(char* one, char* two, char* three, char* four){
    *one = data[0];
    *two = data[1];
    *three = data[2];
    *four = data[3];

}


void UART_Translate(char one, char two, char three, char four, 
    uint32_t* msg, uint32_t* frame, bool* alive, bool* pickup, bool* shot, uint32_t* shotDirection, int32_t* x, int32_t* y){
       *msg = one&0x7F;
       *frame = two&(3<<5);
       *alive = two&(1<<4);
       *pickup = two&(1<<3);
       *shot = two&(1<<2);
       *shotDirection = two&(3);
       *x = three&0x7F;
       *y = four&0x7F;
}



// Formats data correctly and puts it in the Tx register
void UART2_Transmit(uint32_t msg, uint32_t frame, bool alive, bool pickup, bool shot, uint32_t shotDirection, int32_t x, int32_t y){

    // Error Checking on X and Y
    if(x < 0){x = 0;}
    if(y < 0){y = 0;}
    if(x > 128){x = 128;}
    if(y > 200){y = 128;}
    x = (uint32_t)x;
    y = (uint32_t)y;
    if(frame > 8){frame = 8;}
    if(shotDirection > 3){shotDirection = 3;}

    // // Send start of packet + special msg
    // UART2->TXDATA = (1<<7)|(msg&0x7F);
    // UART2->TXDATA = (frame<<5)|(alive<<4)|(pickup<<3)|(shot<<2)|(shotDirection);
    // UART2->TXDATA = x&0x7F;
    // UART2->TXDATA = y&0x7F;

}




extern "C" void UART2_IRQHandler(void);
void UART2_IRQHandler(void){ 
    int volatile status;
    // 1) HeartBeat
    // GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
    static uint32_t U2ISRcount = 0;
    //U2ISRcount = U2ISRcount%4; // circle back to 4
    // 2) HeartBeat
    // GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
    // 3) Add ByteS to Q (if valid data)
    while((UART2->STAT & (1<<2)) == 0){  // see if RXFE is 0 (valid data)
    //while (!((UART2->STAT / 4) % 2)) {
      uint8_t data = UART2->RXDATA;
      data = (char) data;
      bool haveSpace = FIFO2.Put(data);
      if(!haveSpace){LostData ++;}  // if could not put data (FIFO full), inc error count
        //data[U2ISRcount] = (char) UART2->RXDATA;
    }
    // 4) RxCounter ++ (for debugging, = num received msgs)
    //ReceiveCount ++; //uint32_t ReceiveCount,TransmitCount;
    // 5) Acknoledge interrupt flag (clear it)
    // UART2->ICR = 1;    // Clear RTOUT (not the best design - could cause issues b/c other flags could cause interupts)
                          // In the online textbook 
                          // Doesnt build? dont remember  

    U2ISRcount ++;

    status = UART2->CPU_INT.IIDX;  // Reading clars RTOUT (and other RIS bits) // See PG. 285, and 282 of the paper textbook

    // 6) HeartBeat
    // GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)


}


// UART1
// ############################################################################################
// ############################################################################################
// ############################################################################################
// ############################################################################################
// ############################################################################################
// ############################################################################################

void UART1_Init(void){
  UART1->GPRCM.RSTCTL = 0xB1000003;
  UART1->GPRCM.PWREN = 0x26000001;
  Clock_Delay(24); // time for uart to power up
  IOMUX->SECCFG.PINCM[PA8INDEX]  = 0x00000082;
  //IOMUX->SECCFG.PINCM[PA11INDEX]  = 0x00040082; // WRONG PIN NUMBER HERE
  UART1->CLKSEL = 0x08; // bus clock
  UART1->CLKDIV = 0x00; // no divide
  UART1->CTL0 &= ~0x01; // disable UART0
  UART1->CTL0 = 0x00020018;
 // assumes an 80 MHz bus clock
  UART1->IBRD = 21;//   divider = 21+45/64 = 21.703125
  UART1->FBRD = 45; // baud =2,500,000/21.703125 = 115,191
  UART1->LCRH = 0x00000030;
  UART1->CTL0 |= 0x01; // enable UART0


    // // UART1 Initialization
    // UART1->GPRCM.RSTCTL = 0xB1000003;
    // UART1->GPRCM.PWREN = 0x26000001;
    // Clock_Delay(24); // time for uart to power up
    // IOMUX->SECCFG.PINCM[PA8INDEX]  = 0x00000082;
    // UART1->CLKSEL = 0x08; // bus clock
    // UART1->CLKDIV = 0x00; // no divide
    // UART1->CTL0 &= ~0x01; // disable UART0
    // UART1->CTL0 = 0x00020018;
    // // assumes an 80 MHz bus clock
    // UART1->IBRD = 800;//   divider = 800 + 0/64 = 800
    // UART1->FBRD = 0; // baud =2,500,000/800 = 3125
    // UART1->LCRH = 0x00000030;
    // UART1->CTL0 |= 0x01; // enable UART1




}





void Uart1_Transmit_1Byte(char data){
    UART1->TXDATA = data;
    UART1->TXDATA = data;
    UART1->TXDATA = data;
    UART1->TXDATA = data;

}





void UART1_Transmit(uint32_t msg, uint32_t frame, bool alive, bool pickup, bool shot, uint32_t shotDirection, int32_t x, int32_t y){

    // Error Checking on X and Y
    if(x < 0){x = 0;}
    if(y < 0){y = 0;}
    if(x > 128){x = 128;}
    if(y > 200){y = 128;}
    x = (uint32_t)x;
    y = (uint32_t)y;
    if(frame > 7){frame = 7;}
    if(shotDirection > 3){shotDirection = 3;}

    // Send start of packet + special msg
    UART1->TXDATA = (1<<7)|(msg&0x7F);
    UART1->TXDATA = (frame<<5)|(alive<<4)|(pickup<<3)|(shot<<2)|(shotDirection);
    UART1->TXDATA = x&0x7F;
    UART1->TXDATA = y&0x7F;

}