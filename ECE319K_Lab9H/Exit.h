
#ifndef EXIT_H_
#define EXIT_H_
#include <stdint.h>


class Exit{ 
  public: // Data (might update to do friend class stuff)
    int32_t TLx;           // Top right X 
    int32_t TLy;           // Top right Y
    int32_t BRx;           // Bottom left X 
    int32_t BRy;           // Bottom left Y
    uint32_t newPlayerX;    // New Player X 
    uint32_t newPlayerY;    // New Player Y
    uint32_t newPlayerFrame;// New Player Frame (0-8)


  public: // Functions (in the order they would be called)
    Exit(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, uint32_t newPx, uint32_t newPy, uint32_t newPFrame);      // Constructor 
    Exit();
    bool touching(int32_t x, int32_t y, uint32_t h, uint32_t w);  // Is some artibrary rectangle touching or intersection this exit? 
      // X, Y is top left corner

};


#endif
