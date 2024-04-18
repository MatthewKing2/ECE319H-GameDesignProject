
#ifndef FRAME_H_
#define FRAME_H_
#include <stdint.h>
#include "Exit.h"
#include "Wall.h"
#define NumberWalls 15 // Max number of walls (including perimiter)
#define NumberExits 5 // Max number of exits 


class Frame{ 
  public: // Data (might update to do friend class stuff)
    uint32_t frameNumber; // (0-8)
    Exit exits[NumberExits];            
    uint32_t exitsIndex;
    Wall walls[NumberWalls];            
    uint32_t wallsIndex;

  public: // Functions
    Frame(uint32_t frameNumber);        // Constructor 
        // Automatlly creates 4 walls (forming map perimiter) <-- TODO
    void InitWall(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy); 
    void InitExit(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame);

};




#endif

