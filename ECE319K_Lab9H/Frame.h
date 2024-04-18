
#ifndef FRAME_H_
#define FRAME_H_
#include <stdint.h>
#include "Exit.h"
#include "Wall.h"


class Frame{ 
  public: // Data (might update to do friend class stuff)
    uint32_t frameNumber; // (0-8)
    Exit* exits;            
    uint32_t numExits;
    uint32_t exitsIndex;
    Wall* walls;            
    uint32_t numWalls;
    uint32_t wallsIndex;

  public: // Functions
    Frame(uint32_t frameNumber,uint32_t numWalls, uint32_t numExits);      // Constructor 
        // Automatlly creates 4 walls (forming map perimiter) <-- TODO
    void InitWall(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy); 
    void InitWall(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy); 

};




#endif

