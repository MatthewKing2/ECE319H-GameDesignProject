
#ifndef EXIT_H_
#define EXIT_H_
#include <stdint.h>


class Exit{ 
  public: // Data (might update to do friend class stuff)
    int32_t TRx;           // Top right X 
    int32_t TRy;           // Top right Y
    int32_t BRx;           // Bottom left X 
    int32_t BRy;           // Bottom left Y
    uint32_t newPlayerX;    // New Player X 
    uint32_t newPlayerY;    // New Player Y
    uint32_t newPlayerFrame;// New Player Frame (0-8)


  public: // Functions (in the order they would be called)
    Exit(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame);      // Constructor 
    bool touching(uint32_t x, uint32_t y);  // Based on player X and Y position determine if they're touching 

};


#endif
