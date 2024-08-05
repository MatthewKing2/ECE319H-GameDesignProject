
//------------------------------------------------------------------------------
// File: GameUart.cpp
// Author: Matthew King
// Description: Contains implementation for UART communication functions,
//              including initialization and data handling for UART1 and UART2.
// Dependencies: GameUart.h, Clock.h, LaunchPad.h, FIFO2.h
//------------------------------------------------------------------------------

#include <ti/devices/msp/msp.h>
#include "GameUart.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/FIFO2.h"
uint32_t LostData = 0;    // Count amount of lost data (for testing)
Queue FIFO1;            // Not in Use
Queue FIFO2;            // Holds RX UART Data (software FIFO)
Queue FIFO3;            // Not in Use
char data[4]; // Not in use


// Receiver Class
// ############################################################################################

//------------------------------------------------------------------------------
// Function: Receiver::Receiver()
// Description: Constructor for the Receiver class. Initializes the frame 
//              number to an out-of-range value to indicate no valid frame 
//              initially.
//------------------------------------------------------------------------------
Receiver::Receiver(){
    this->frame = 10;
}

//------------------------------------------------------------------------------
// Function: Receiver::receiverTranslate(char one, char two, char three, char four)
// Description: Decodes a 4-byte packet into Receiver class attributes using 
//              bitwise operations to extract the fields from each byte.
// Packet format:
//  - Byte One:
//     - [7]   = 1 (start of packet bit)
//     - [6:0] = Special Messages
//  - Byte Two:
//     - [7]   = 0 
//     - [6:4] = Frame Number
//     - [3]   = Alive Boolean
//     - [2]   = Shot Boolean
//     - [1:0] = Shot Direction
//  - Byte Three:
//     - [7]   = 0 
//     - [6:0] = Player X Position 
//  - Byte Four:
//     - [7]   = 0 
//     - [6:0] = Player Y Position 
//------------------------------------------------------------------------------
void Receiver::receiverTranslate(char one, char two, char three, char four){
    this->msg = one&0x7F;
    this->frame = (two&(7<<4))>>4;  
    this->alive = (two&(1<<3))>>3;
    this->pickup = 0;               
    this->shot = (two&(1<<2))>>2;
    this->shotDirection = two&(3);  
    this->x = three&0x7F;
    this->y = four&0x7F;
}


// UART1 and UART2
// ############################################################################################

//------------------------------------------------------------------------------
// Function: UART2_Init(void)
// Description: Initializes UART2 with a 115200 baud rate. Configures pins, 
//              clock, and UART settings. Enables receive interrupt on timeout.
// Note: For 80MHz bus clock, UART2 clock is ULPCLK 40MHz
//------------------------------------------------------------------------------
void UART2_Init(void){
  UART2->GPRCM.RSTCTL = 0xB1000003;               // reset UART0
  UART2->GPRCM.PWREN = 0x26000001;                // activate UART0
  Clock_Delay(24);                                // time for uart to activate
  IOMUX->SECCFG.PINCM[PA24INDEX] = 0x00040082;    // Regular Input PA24 = Rx
  UART2->CLKSEL = 0x08;               // bus clock
  UART2->CLKDIV = 0x00;               // no divide
  UART2->CTL0 &= ~0x09;               // was 1, disable uart
  UART2->CTL0 = 0x00020018;           // enable fifos, tx and rx
  UART2->IBRD = 21;                   // From Textbook 115200
  UART2->FBRD = 45;
  UART2->LCRH = 0x00000030;           // 8bit, 1 stop, no parity
  UART2->CPU_INT.IMASK = 0x0001;      // Just RTOUT (see pg. 286 for TXINT and RXINT, bits 11,10 respectivly)
  UART2->IFLS = 0x422;
  NVIC->ICPR[0] = 1<<14;
  NVIC->ISER[0] = 1<<14;
  NVIC->IP[3] = (NVIC->IP[3]& (~0x00FF0000))|(2<<22);   // Set Priority 
  UART2->CTL0 |= 0x09;
}

//------------------------------------------------------------------------------
// Function: UART2_InChar(void)
// Description: Retrieves a character from the UART2 receive software FIFO. Returns 
//              0 if no valid data is available in FIFO2.
//------------------------------------------------------------------------------
char UART2_InChar(void){char out;
  char data;
  bool valid = FIFO2.Get(&data);
  if(valid){return data;}
  return 0;
}

//------------------------------------------------------------------------------
// Function: UART2_IRQHandler(void)
// Description: Handles UART2 interrupts by reading data from the receive hardware FIFO 
//              and incrementing the LostData counter if the hardware FIFO is full.
//------------------------------------------------------------------------------
extern "C" void UART2_IRQHandler(void);
void UART2_IRQHandler(void){ 
    int volatile status;
    static uint32_t U2ISRcount = 0; // Count the number of times this ISR gets called
    
    // Wait until valid data in UART2's hardware RX FIFO
    // If the software FIFO does not have room for the new data (from hardware FIFO), increase count of lost data
    while((UART2->STAT & (1<<2)) == 0){  // RXFE = 0 (valid data)
      uint8_t data = UART2->RXDATA;
      data = (char) data;

      // See if SW FIFO has space
      bool haveSpace = FIFO2.Put(data);
      if(!haveSpace){LostData ++;}      
    }

    U2ISRcount ++;
    status = UART2->CPU_INT.IIDX;  // Reading clears RTOUT (and other RIS bits, See PG. 285, and 282 of the textbook)
}

//------------------------------------------------------------------------------
// Function: UART1_Init(void)
// Description: Initializes UART1 with a 115200 baud rate. Configures pins, 
//              clock, and UART settings. Assumes an 80 MHz bus clock.
//------------------------------------------------------------------------------
void UART1_Init(void){
  UART1->GPRCM.RSTCTL = 0xB1000003;
  UART1->GPRCM.PWREN = 0x26000001;
  Clock_Delay(24);                              // Wait for UART to power up
  IOMUX->SECCFG.PINCM[PA8INDEX]  = 0x00000082;
  UART1->CLKSEL = 0x08;                         // Bus clock
  UART1->CLKDIV = 0x00;                         // No divide
  UART1->CTL0 &= ~0x01;                         // Disable UART1
  UART1->CTL0 = 0x00020018;
  UART1->IBRD = 21;                             // Divider = 21+45/64 = 21.703125
  UART1->FBRD = 45;                             // Baud = 2,500,000/21.703125 = 115,191
  UART1->LCRH = 0x00000030;
  UART1->CTL0 |= 0x01;                          // enable UART1

}

//------------------------------------------------------------------------------
// Function: UART1_Transmit(uint32_t msg, uint32_t frame, bool alive, bool pickup, bool shot, uint32_t shotDirection, int32_t x, int32_t y)
// Description: Formats and transmits a 4-byte packet over UART1 with the given 
//              data. Handles error checking for data to ensure they fall within 
//              acceptable ranges.
//------------------------------------------------------------------------------
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
    UART1->TXDATA = ((frame<<4)|(alive<<3)|(shot<<2)|(shotDirection))&0x7F; 
    UART1->TXDATA = x&0x7F;
    UART1->TXDATA = y&0x7F;

}


// Not Being Used
// ############################################################################################

//------------------------------------------------------------------------------
// Note: The following functions are not currently in use and are provided 
//       for reference or future implementation. They are not invoked in 
//       the current setup or code base:
//------------------------------------------------------------------------------
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
void UART2_Transmit(uint32_t msg, uint32_t frame, bool alive, bool pickup, bool shot, uint32_t shotDirection, int32_t x, int32_t y){
    if(x < 0){x = 0;}
    if(y < 0){y = 0;}
    if(x > 128){x = 128;}
    if(y > 200){y = 128;}
    x = (uint32_t)x;
    y = (uint32_t)y;
    if(frame > 8){frame = 8;}
    if(shotDirection > 3){shotDirection = 3;}
}
void Uart1_Transmit_1Byte(char data){
    UART1->TXDATA = data;
    UART1->TXDATA = data;
    UART1->TXDATA = data;
    UART1->TXDATA = data;
}
