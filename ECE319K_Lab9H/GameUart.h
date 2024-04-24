
#include <stdint.h>

// #include "../inc/ST7735.h"

//#include "GameFifo.h"

#include "../inc/FIFO2.h"

#ifndef GAMEUART_H_
#define GAMEUART_H_

// #define FIFOSIZE 32  // maximum storage is FIFO_SIZE-1 elements 
// class Queue{ 
// private:
//   char Buf[FIFOSIZE];
//   int PutI; // index to an empty place, next place to put
//   int GetI; // index to oldest data, next to get

// public:
//   Queue();            // initialize queue
//   bool IsEmpty(void); // true if empty
//   bool IsFull(void);  // true if full
//   bool Put(char x);   // enter data into queue
//     // Flase = Q Full
//     // True = Sucess
//   bool Get(char* pt); // remove data from queue (put at pointer location)
//     // False = Q Empty
//     // True = Sucess
//   void Print(void);   // display element of queue on LCD
// };



// // A class named Queue that defines a FIFO
// Queue::Queue(){
//     PutI = FIFOSIZE-1;
//     GetI = FIFOSIZE-1;
//   // Constructor - set PutI and GetI as 0. 
//   // We are assuming that for an empty Queue, both PutI and GetI will be equal

// // add code here to initialize on creation
// }

// // To check whether Queue is empty or not
// bool Queue::IsEmpty(void){
//   return (PutI == GetI);  
// }

//   // To check whether Queue is full or not
// bool Queue::IsFull(void){
//   int decrimentedPutI = PutI - 1;
//   if(decrimentedPutI < 0){decrimentedPutI = FIFOSIZE-1;}
//   return (decrimentedPutI == GetI);  
// }

//   // Inserts an element in queue at rear end
// bool Queue::Put(char x){
//   // if Q is full, return Error Code (false)
//   if(IsFull()){return false;} 
//   // if Q has room, add char X
//   Buf[PutI] = x;
//   PutI --;
//   if(PutI < 0){PutI = FIFOSIZE-1;}
//   // Return Sucess code
//   return true;  

// }

//   // Removes an element in Queue from front end. 
// bool Queue::Get(char* pt){

//   // if Q is empty, return Error Code (false)
//   if(IsEmpty()){return false;}

//   // if Q has data, return First In
//   //1) Pop the element
//   *(pt) = Buf[GetI];
//   // 2) Decriment Index
//   GetI --;
//   // 3) Wrap Index (if applicable)
//   if(GetI < 0){GetI = FIFOSIZE-1;}

//   // Return sucess code
//   return true;  

// }

//   /* 
//      Printing the elements in queue from front to rear. 
//      This function is only to test the code. 
//      This is not a standard function for Queue implementation. 
//   */
// void Queue::Print(void){
//     // Finding number of elements in queue  
//   // output to ST7735R

//     if(IsFull()){
//         for(int i = FIFOSIZE-1; i >=0 ;i--){
//            ST7735_OutChar(Buf[i] + 0x30);
//         }
//     }
//     else{
//         for(int i = GetI; i >= PutI; i--){
// //            if(Buf[i] > 47 && Buf[i] < 58){
// //                ST7735_OutChar(Buf[i]+0x30);
// //            }
//             ST7735_OutChar(Buf[i]+0x30);
//         }
//     }




// }









// class GameUart{ 
//   public: // Data 
//     int32_t x;         // Player X and Y position 
//     int32_t y;
//     int32_t prevX;         // Player X and Y position 
//     int32_t prevY;
//     uint16_t h,w;       // Player Width and Hight 
//     uint32_t color;     // Player color
//     const uint16_t* image;    // What player looks like
//     bool alive;         // Is player alive
//     bool murder;        // Is the player the murderur 

//   public: // Functions (in the order they would be called)
//     GameUart();          // Constructor 
//     // Translate UART msgs
//     void UART_Translate(char one, char two, char three, char four);


//     // Initalize Uarts
//     void UART0_Init(void);
//     void UART1_Init(void);
//     void UART2_Init(void);

//     // Receive from Uarts (using software FIFO)
//     char UART0_InChar(void);
//     char UART1_InChar(void);
//     char UART2_InChar(void);

//     // Transmit via Uarts
//     char UART0_Transmit(char data, uint32_t frameNum);
//     char UART1_Transmit(char data, uint32_t frameNum);
//     char UART2_Transmit(uint32_t msg, uint32_t frame, bool alive, bool pickup, bool shot, uint32_t shotDirection, int32_t x, int32_t y);
// };



class Receiver{ 
  public: // Data 

    uint32_t msg;         
    uint32_t frame;
    bool alive;
    bool pickup;
    bool shot;
    uint32_t shotDirection;
    int32_t x;
    int32_t y;

  public: // Functions (in the order they would be called)
    Receiver(); // Default COnstructr
    void receiverTranslate(char one, char two, char three, char four);

};






// Translate UART msgs
void UART_Translate(char one, char two, char three, char four);


// Initalize Uarts
void UART0_Init(void);
void UART1_Init(void);
void UART2_Init(void);

// Receive from Uarts (using software FIFO)
char UART0_InChar(void);
char UART1_InChar(void);
char UART2_InChar(void);

// Transmit via Uarts
char UART0_Transmit(char data, uint32_t frameNum);
void UART1_Transmit(uint32_t msg, uint32_t frame, bool alive, bool pickup, bool shot, uint32_t shotDirection, int32_t x, int32_t y);
void UART2_Transmit(uint32_t msg, uint32_t frame, bool alive, bool pickup, bool shot, uint32_t shotDirection, int32_t x, int32_t y);



void UART2_InData(char* one, char* two, char* three, char* four);
void Uart1_Transmit_1Byte(char data);



#endif 

