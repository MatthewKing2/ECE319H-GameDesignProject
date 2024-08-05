
//------------------------------------------------------------------------------
// File: LCDDisplay.h
// Author: Matthew King
// Description: Header file for the LCD class responsible for managing and 
//              displaying different game elements on the screen. Includes 
//              functions to update the display with new frames, player, enemies, 
//              and shots.
// Dependencies: Frame.h, Player.h, Enemy.h
//------------------------------------------------------------------------------ 
#ifndef LCDISPLAY_H_
#define LCDISPLAY_H_
#include <stdint.h>
#include "Frame.h"
#include "Player.h"
#include "Enemy.h"


class LCD{ 
  public: 
    bool FrameShift;          // Semaphore indicating whether a frame shift is needed
    uint32_t oldFrame;        // Index of the old frame to be erased
    uint32_t newFrame;        // Index of the new frame to be displayed
    bool DisplayReady;        // Semaphore indicating if the display is ready for updates
    uint16_t backgroundColor; // Color used to clear the background
    uint16_t wallColor;       // Color used to draw walls
    uint16_t exitColor;       // Color used to draw exits
    uint16_t shotColor;       // Color used to draw shots

  public: 
    //------------------------------------------------------------------------------  
    // Constructor: LCD()
    // Description: Initializes the LCD object with default values including 
    //              semaphores, frame indices, and colors for various elements.
    //------------------------------------------------------------------------------
    LCD();

    //------------------------------------------------------------------------------  
    // Function: void displayNewScreen(uint32_t currFrame)
    // Description: Initializes the screen with the new frame. Draws walls and 
    //              exits based on the current frame, using specific colors.
    //              Noteworthy:
    //              - Exits are drawn over walls with a different color.
    //------------------------------------------------------------------------------
    void displayNewScreen(uint32_t currFrame);

    //------------------------------------------------------------------------------  
    // Function: void frameShift(Player& p1)
    // Description: Erases the old frame and displays the new one. Updates the 
    //              screen to reflect the new state of the game, including player 
    //              and enemy positions.
    //              Noteworthy:
    //              - Clears old enemies and player from the display.
    //              - Draws new walls and exits based on the updated frame.
    //------------------------------------------------------------------------------
    void frameShift(Player&);

    //------------------------------------------------------------------------------  
    // Function: void displayPlayer(Player& p1)
    // Description: Draws the player on the screen at the current position with 
    //              the specified image. 
    //              Noteworthy:
    //              - Currently uses bitmap drawing instead of filling a rectangle.
    //------------------------------------------------------------------------------
    void displayPlayer(Player&);

    //------------------------------------------------------------------------------  
    // Function: void displayEnemies(uint32_t currFrame)
    // Description: Displays all enemies for the current frame. Updates the 
    //              display to reflect changes in enemy positions and visibility.
    //              Noteworthy:
    //              - Clears the previous position of enemies that have changed frames.
    //              - Draws enemies if their frame matches the current frame.
    //------------------------------------------------------------------------------
    void displayEnemies(uint32_t currFrame);

    //------------------------------------------------------------------------------  
    // Function: void clearSpecificEnemy(Enemy& e1)
    // Description: Clears a specific enemy. This function is not being used.
    //------------------------------------------------------------------------------
    void clearSpecificEnemy(Enemy& e1);

    //------------------------------------------------------------------------------  
    // Function: void displayShots(uint32_t currentFrame)
    // Description: Displays all valid shots on the screen for the given frame.
    //              Uses the shot color to draw the rectangles representing shots.
    //              Noteworthy:
    //              - Only draws shots that are valid and correspond to the current frame.
    //------------------------------------------------------------------------------
    void displayShots(uint32_t currentFrame);

    //------------------------------------------------------------------------------  
    // Function: void clearShots(uint32_t currentFrame)
    // Description: Clears all valid shots from the screen for the given frame 
    //              and marks them as invalid. 
    //              Noteworthy:
    //              - Resets the valid status of shots to false after erasing them.
    //------------------------------------------------------------------------------ 
    void clearShots(uint32_t currentFrame);

};

#endif
