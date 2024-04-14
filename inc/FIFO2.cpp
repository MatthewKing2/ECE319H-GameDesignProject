// FIFO2.cpp
// Runs on any microcontroller
// Provide functions that initialize a FIFO, put data in, get data out,
// and return the current size.  The file includes a transmit FIFO
// using index implementation and a receive FIFO using pointer
// implementation.  Other index or pointer implementation FIFOs can be
// created using the macros supplied at the end of the file.
// Created: 1/16/2020 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly


#include <stdint.h>

#include "../inc/FIFO2.h"
#include "../inc/ST7735.h"


// A class named Queue that defines a FIFO
Queue::Queue(){
    PutI = FIFOSIZE-1;
    GetI = FIFOSIZE-1;
  // Constructor - set PutI and GetI as 0. 
  // We are assuming that for an empty Queue, both PutI and GetI will be equal

// add code here to initialize on creation
}

// To check whether Queue is empty or not
bool Queue::IsEmpty(void){
  return (PutI == GetI);  
}

  // To check whether Queue is full or not
bool Queue::IsFull(void){
  int decrimentedPutI = PutI - 1;
  if(decrimentedPutI < 0){decrimentedPutI = FIFOSIZE-1;}
  return (decrimentedPutI == GetI);  
}

  // Inserts an element in queue at rear end
bool Queue::Put(char x){
  // if Q is full, return Error Code (false)
  if(IsFull()){return false;} 
  // if Q has room, add char X
  Buf[PutI] = x;
  PutI --;
  if(PutI < 0){PutI = FIFOSIZE-1;}
  // Return Sucess code
  return true;  

}

  // Removes an element in Queue from front end. 
bool Queue::Get(char* pt){

  // if Q is empty, return Error Code (false)
  if(IsEmpty()){return false;}

  // if Q has data, return First In
  //1) Pop the element
  *(pt) = Buf[GetI];
  // 2) Decriment Index
  GetI --;
  // 3) Wrap Index (if applicable)
  if(GetI < 0){GetI = FIFOSIZE-1;}

  // Return sucess code
  return true;  

}

  /* 
     Printing the elements in queue from front to rear. 
     This function is only to test the code. 
     This is not a standard function for Queue implementation. 
  */
void Queue::Print(void){
    // Finding number of elements in queue  
  // output to ST7735R

    if(IsFull()){
        for(int i = FIFOSIZE-1; i >=0 ;i--){
           ST7735_OutChar(Buf[i] + 0x30);
        }
    }
    else{
        for(int i = GetI; i >= PutI; i--){
//            if(Buf[i] > 47 && Buf[i] < 58){
//                ST7735_OutChar(Buf[i]+0x30);
//            }
            ST7735_OutChar(Buf[i]+0x30);
        }
    }




}


