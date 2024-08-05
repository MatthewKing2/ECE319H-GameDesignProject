//------------------------------------------------------------------------------
// File: GameUART.h
// Author: Matthew King
// Description: Header file for UART communication and data handling in the 
//              game. Defines the `Receiver` class for parsing received data 
//              and functions for initializing and using UART modules. Includes 
//              functionalities for transmitting and receiving data through 
//              UART1 and UART2.
// Dependencies: FIFO2.h
//------------------------------------------------------------------------------ 
#include <stdint.h>
#include "../inc/FIFO2.h"
#ifndef GAMEUART_H_
#define GAMEUART_H_

class Receiver{ 
  public: 
    uint32_t msg;             // Special Messages (never got implimented)
    uint32_t frame;           // What frame (room) the enemy player is in
    bool alive;               // Is the enemy player alive
    bool pickup;              // Did the enemy pick up an item (never got implimented)
    bool shot;                // Did the enemy player shoot
    uint32_t shotDirection;   // What direction did they shoot in
    int32_t x;                // Enemy X position (top left)
    int32_t y;                // Enemy Y position (top left)

  public:
    //------------------------------------------------------------------------------
    // Function: Receiver::Receiver()
    // Description: Default constructor for the Receiver class.
    //------------------------------------------------------------------------------
    Receiver();

    //------------------------------------------------------------------------------
    // Function: Receiver::receiverTranslate(char one, char two, char three, char four)
    // Description: Translates received characters into meaningful game data. 
    //              This function is responsible for parsing UART data into 
    //              relevant fields such as shot direction, position, and state.
    // Parameters: 
    //              - char one: First character of the received data. // Change this to explain the data format
    //              - char two: Second character of the received data.
    //              - char three: Third character of the received data.
    //              - char four: Fourth character of the received data.
    //------------------------------------------------------------------------------
    void receiverTranslate(char one, char two, char three, char four);

};

//------------------------------------------------------------------------------
// Function: UART1_Init(void)
// Description: Initializes UART1 for transmitting data. Configures UART1 
//              settings such as baud rate, data format, and enables the 
//              transmitter.
//------------------------------------------------------------------------------
void UART1_Init(void);  // Tx UART

//------------------------------------------------------------------------------
// Function: UART2_Init(void)
// Description: Initializes UART2 for receiving data. Configures UART2 
//              settings such as baud rate, data format, and enables the 
//              receiver.
//------------------------------------------------------------------------------
void UART2_Init(void);  // Rx UART

//------------------------------------------------------------------------------
// Function: char UART2_InChar(void)
// Description: Retrieves a byte from a software receive FIFO. This software FIFO
//              gets populated by UART2 using an ISR (defined in GameUart.cpp).
// Returns: Byte from software receive FIFO
//------------------------------------------------------------------------------
char UART2_InChar(void); 

//------------------------------------------------------------------------------
// Function: void UART1_Transmit(uint32_t msg, uint32_t frame, bool alive, bool pickup, bool shot, uint32_t shotDirection, int32_t x, int32_t y)
// Description: Transmits data via UART1. This function sends a structured 
//              message containing game data such as the current frame, player 
//              status, shot information, and coordinates.
// Parameters:
//  - uint32_t msg: Message identifier or content to be transmitted.
//  - uint32_t frame: Frame number associated with the message.           // Update this to be correct
//  - bool alive: Status indicating whether the player is alive.
//  - bool pickup: Status indicating whether a pickup event occurred.
//  - bool shot: Status indicating whether a shot event occurred.
//  - uint32_t shotDirection: Direction of the shot (0=Up, 1=Down, 2=Left, 3=Right).
//  - int32_t x: X-coordinate of the shot or player.
//  - int32_t y: Y-coordinate of the shot or player.
//------------------------------------------------------------------------------
void UART1_Transmit(uint32_t msg, uint32_t frame, bool alive, bool pickup, bool shot, uint32_t shotDirection, int32_t x, int32_t y);  // Using this one

//------------------------------------------------------------------------------
// Note: The following functions are not currently in use and are provided 
//       for reference or future implementation. They are not invoked in 
//       the current setup or code base:
//------------------------------------------------------------------------------
void UART_Translate(char one, char two, char three, char four); // Never got fully implimented 
void UART0_Init(void);                                          // Not using UART0
char UART0_InChar(void);                                        // Not using UART0
char UART1_InChar(void);                                        // UART1 is only a transmitter (so this receive function is not used)
char UART0_Transmit(char data, uint32_t frameNum);              // Not using UART0
void UART2_Transmit(uint32_t msg, uint32_t frame, bool alive, bool pickup, bool shot, uint32_t shotDirection, int32_t x, int32_t y); 
                                                                    // UART2 is only a receiver (so this transmit function is not used)
void UART2_InData(char* one, char* two, char* three, char* four);   // Never got fully implimented
void Uart1_Transmit_1Byte(char data);                               // Was used only for testing

#endif 
