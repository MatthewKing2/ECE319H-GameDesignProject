
#ifndef MYBUTTONS_H_
#define MYBUTTONS_H_
#include <stdint.h>


class Buttons{ 
  private: // Data 
    // None rn

  public: // Functions 
    Buttons();                  // Constructor
    void Buttons_Init(void);    // Init the joystick
    void Buttons_In(uint32_t* up, uint32_t* down, uint32_t* left, uint32_t* right, uint32_t* dash); 
                                // Return button values for each button(either 0 or 1


    // Simons Code:
    bool Buttons_Pressed();
    void Menu_Buttons_In(uint32_t* up, uint32_t* down, uint32_t* left, uint32_t* right, uint32_t* dash); 

};



#endif
