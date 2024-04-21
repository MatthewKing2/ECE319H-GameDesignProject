

#ifndef WALL_H_
#define WALL_H_
#include <stdint.h>


class Wall{ 
  public: // Data (might update to do friend class stuff)
    int32_t TLx;           // Top right X 
    int32_t TLy;           // Top right Y
    int32_t BRx;           // Bottom left X 
    int32_t BRy;           // Bottom left Y

  public: // Functions (in the order they would be called)
    Wall(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy);      // Constructor 
    Wall();
    bool touching(int32_t x, int32_t y, uint32_t h, uint32_t w);  // Based on player X and Y position determine if they're touching 

};


#endif