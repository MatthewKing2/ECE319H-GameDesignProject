# ECE319H-GameDesignProject

## Overview

**Cantina Wars** is a Star Wars inspired handheld two person retro style video game developed as part of ECE319H at UT Austin. The game is designed to run on a custom PCB that uses a TI LaunchPad (MSPM0G3507 microcontroller) and features various peripherals including an LCD, sound (DAC), joystick (ADC), buttons, and LEDs. The game presents a 2D top-down action arcade game with multiple rooms and different obstacles to create a stagegic Han Solo vs Boba Fett combat experience.

This repository contains the key software written for the game including the game logic and hardware drivers.

## [YouTube Demo](https://www.youtube.com/watch?v=pERT4R3idnI)

[![Watch the YouTube Demo](./CantinaWarsThumbnail.png)](https://www.youtube.com/watch?v=pERT4R3idnI)

## Technical Features

- **Symmetric UART Communication**: Implements bidirectional UART communication between microcontrollers, creating a networked multiplayer experience where each microcontroller perceives the other as an enemy. The exact same code (excluding character sprites) runs on both microntrollers.

- **Custom Data Packet Format**: Utilizes a custom-defined four byte packet structure for efficient data transmission and communication between microcontrollers, ensuring reliable game state updates.

- **Multiple Threads**: Incorporates multitasking with dedicated threads for display updates (Main Thread), game engine (ISR), sound (ISR), and UART receive (ISR) allowing smooth and responsive gameplay.

- **Custom PCB**: Designed and fabricated a custom PCB that interfaces with the TI LaunchPad and microcontroller, integrating components such as an LCD, sound (DAC), joystick (ADC), buttons, LEDs, and UART communication for a comprehensive game system.

- **Extended Game Map**: Features a dynamic map larger than the LCD screen, with multiple interconnected rooms and handling of walls and exits, providing an immersive and expansive gameplay experience (Only game in the class with this feature).

## Key Files

### `Lab9HMain.cpp`

Contains the core implementation of the game: initializing the hardware, setting up the game environment, and managing the main game loop. Key functionalities:

- **Game Setup**: Sets up the hardware: microcontroller clock, UART communication, display, buttons, joystick, and sound. Also initializes game frames (rooms), walls, exits, and player/enemy objects.
- **Display Management**: Controls the game's LCD display, updating the screen with the current game state and handling transitions between game screens (win, game in progress, lose).
- **Game Engine**: This 30Hz interrupt service routine manages game state updates, including checking player health, handling frame (room) transitions, processing player input, managing shooting mechanics, transmitting data, and receiving data from the other player's microcontroller.

### `GameUart.cpp` / `GameUart.h`

Implements UART communication functions used for sending and receiving data between the game and external devices. This includes initialization of UART modules and data handling.

### `Player.cpp` / `Player.h`

Contains the implementation and interface for the `Player` class. This class handles player initialization, movement, and interactions within the game. It includes methods for player movement, collision detection, and handling player states.

### `LaserShot.cpp` / `LaserShot.h`

Defines the `LaserShot` class responsible for creating and managing laser shots fired by the player and enemies. This includes methods for initializing, updating, and rendering laser shots on the screen.

### `Frame.cpp` / `Frame.h`

Manages the different frames or rooms of the game. The `Frame` class includes methods for initializing walls, exits, and other environmental elements. It also handles room transitions.

## Game Flow

The following illustrates the flow of the two key threads in the game.

### **Display Managment (Main Thread)**

- Wait for "Display Ready" Semaphore
- If Player Won, Display Win Screen
- If Game in Progress, Display Game Screen
- If Player Lose, Display Lose Screen
- Clear "Display Ready" Semaphore
- Repeat!

### **Game Engine (ISR)**

Every 30Hz ISR is called:

- Is player Alive?
- Delete old shots
- Handle player exiting frame
- Take player input
- Move player
- Handle Shooting
- Transmit my information
- Receive the enemy's information
- Set "Display Ready" Semaphore
- Repeat!

## Acknowledgments

- **My Group Partern**: Simon Knuff for handling sound, the menu screens, and helping with the PCB design.
- **Professors and TAs**: Dr. Tiwari, Dr. Valvano for providing guidance and support throughout the project.
- **Texas Instruments**: For the microcontroller related libraries.

---
