
#ifndef LASERSHOT_H_
#define LASERSHOT_H_
#include <stdint.h>


class Shot{ 
  public: // Data (Mailbox system) 
    int32_t TLx;           // Top right X 
    int32_t TLy;           // Top right Y
    int32_t BRx;           // Bottom left X 
    int32_t BRy;           // Bottom left Y
    uint32_t frame;        // What frame (index) is shot in
    bool valid;            // Semaphor

  public: // Functions 
    Shot(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, bool valid);   // Testing Constructor 
    Shot(void);                                                             // Default Constructor
    bool touching(int32_t x, int32_t y, uint32_t h, uint32_t w);            // Based on player X and Y position determine if they're touching 
    void generate(int32_t x, int32_t y, uint32_t direction, uint32_t frame);// Generates a new shot until a wall is hit (updates its data)
    void cleanUp(uint32_t currFrame);                                       // Clean up any valid shots in your frame 
};


#endif