


#include <ti/devices/msp/msp.h>
#include "LaserShot.h"
#include "../inc/ST7735.h"
#include "Frame.h"

extern Frame frames[];


// class Shot{ 
//   public: // Data (Mailbox system) 
//     int32_t TLx;           // Top right X 
//     int32_t TLy;           // Top right Y
//     int32_t BRx;           // Bottom left X 
//     int32_t BRy;           // Bottom left Y
//     bool valid;            // Semaphor

//   public: // Functions (in the order they would be called)
//     Shot(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, bool valid);                           // Constructor 
//     bool touching(int32_t x, int32_t y, uint32_t h, uint32_t w, int32_t* Xover, int32_t* Yover);    // Based on player X and Y position determine if they're touching 
//     void generate(int32_t x, int32_t y, uint32_t direction);                                        // Generates a new shot until a wall is hit (updates its data)

// };


// Non-Default Constructor (called for testing)
Shot::Shot(int32_t TRx, int32_t TRy, int32_t BRx, int32_t BRy, bool valid){
    this->TLx = 0;
    this->TLy = 0;
    this->BRx = 0;
    this->BRy = 0;
    this->frame = 10;      // Invalid index (prevent stupid errors)
    this->valid = false;
}                           


// Default Constuctor (called when global array created)
Shot::Shot(){
    this->TLx = 0;
    this->TLy = 0;
    this->BRx = 0;
    this->BRy = 0;
    this->frame = 10;      // Invalid Index (prevent stupid erros) 
    this->valid = false;
}

// Given some Player X,Y and H,W, dtermine if they are touching the shot (and die) 
bool Shot::touching(int32_t x, int32_t y, uint32_t h, uint32_t w){ 
    // Same code as Exit's touching
    if( ((x+w-1 < TLx)||(x > BRx))   ||   ((y+h-1 < TLy)||(y > BRy)) ){
      return false; 
    }
    return true; 
}



// Given the shot direction, and Player X,Y generate a new shot (until it hits a wall)
// Direction
    // 0 = Up
    // 1 = Down
    // 2 = Left
    // 3 = Right
void Shot::generate(int32_t x, int32_t y, uint32_t direction, uint32_t currFrameIndex){

    // Increase the coresponding axis by 4 until it hits a wall 
        // Use wall's touching boolean, and overX, overY to stop and resize 

    // Player Height and Width
    uint32_t w = 8;
    uint32_t h = 8;

    // Not currently using these vars, just for funtion calls :)
    int32_t Xover;
    int32_t Yover;

    // bool Wall::touching(int32_t x, int32_t y, uint32_t h, uint32_t w, int32_t* Xover, int32_t* Yover){
        // Basically just gonna change the height (or wideth) var, untill we hit 

    // Up generate 
    if(direction == 0){
        // TLy will change 
        this->TLx = x;
        this->TLy = y;
        this->BRx = x+w-1;
        this->BRy = y;
        // Change Height by 4 over and over (width no change)
        bool touching = false;
        int32_t deltaH = 0;
        while(!touching && (deltaH > -128)){
            for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
                touching |= frames[currFrameIndex].walls[i].touching(TLx, TLy+deltaH, -1*deltaH, w, &Xover, &Yover);
            }
            deltaH -= 1;
        }

        this->TLy += deltaH+1; 
    }


    // Down generate 
    else if(direction == 1){
        // BRy will change 
        this->TLx = x;
        this->TLy = y+h-1;
        this->BRx = x+w-1;
        this->BRy = y+h-1;
        // Change Height by 4 over and over (width no change)
        bool touching = false;
        int32_t deltaH = 0;
        while(!touching && (deltaH < 128)){
            for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
                touching |= frames[currFrameIndex].walls[i].touching(this->TLx, this->TLy, deltaH, w, &Xover, &Yover);
            }
            deltaH += 1;
        }

        this->BRy += deltaH-1; 
    }



    // Left generate 
    if(direction == 2){
        // TLx will change 
        this->TLx = x;
        this->TLy = y;
        this->BRx = x;
        this->BRy = y+h-1;
        // Change Height by 4 over and over (width no change)
        bool touching = false;
        int32_t deltaW = 0;
        while(!touching && (deltaW > -128)){
            for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
                touching |= frames[currFrameIndex].walls[i].touching(TLx+deltaW, TLy, h, -1*deltaW, &Xover, &Yover);
            }
            deltaW -= 1;
        }

        this->TLx += deltaW+1; 
    }


    // Right generate 
    else if(direction == 3){
        // BRx will change 
        this->TLx = x+w-1;
        this->TLy = y;
        this->BRx = x+w-1;
        this->BRy = y+h-1;
        // Change Height by 4 over and over (width no change)
        bool touching = false;
        int32_t deltaW = 0;
        while(!touching && (deltaW < 128)){
            for(int i = 0; i <= frames[currFrameIndex].wallsIndex; i ++){
                touching |= frames[currFrameIndex].walls[i].touching(TLx, TLy, h, deltaW, &Xover, &Yover);
            }
            deltaW += 1;
        }

        this->BRx += deltaW-1; 
    }


    

    // Update Mailbox info 
    this->frame = currFrameIndex;
    this->valid = true;

}
