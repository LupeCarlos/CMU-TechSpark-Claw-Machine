# CMU-TechSpark-Claw-Machine
Carnegie Mellon University's very own claw machine. (well, this is the code for that.)

This repo consists of four files:
1. Claw_Machine.ino                 - Contains setup and loop functions, includes below three files with constants and functions
2. Claw_Game_Control_Functions.h    - Contains functions that control game setup, timing, and ending 
4. Claw_Movement_Functions.h        - Contains functions that control claw movement
3. Claw_Machine_Constants.h         - Contains constants used by functions

Claw_Machine.ino FILE:

The overall structure is contained in the top level file Claw_Machine.ino. Most of the inputs from the claw buttons are handled in this file,
and are distributed to different game functions based on what they are used for. Ie. Start button's input is sent to functions in
Claw_Game_Control_Functions.h, and direction inputs are sent to functions in Claw_Movement_functions.h. 

This claw system relies on serveral different sets of inputs:

Claw Movment inputs:
1. X, Y, Z positive and negative values (Z controlled by two separate buttons, X and Y controlled by joystick)
2. X, Y, minimum/maximum direction detection. Z minimum direction detection. (so directions aren't driven outside of bounds)
3. Claw Open/Close motion driving (Controlled by two separate buttons)

Directional information is given only to active movement functions as an and: the claw movement direction must be high AND the must not be
hitting the edge in the direction they are trying to go.

Claw Control inputs
4. Game Start button -- if pressed again after 5 seconds since game has started, will automatically end game
5. Game mode -- switch between FULL CONTROL mode and ARCADE MODE
6. Potentiometer -- for active game length control

Claw_Machine_Constants.h FILE:

Contains all pin assignment constants for Arduino. Contains display setup information and constant display writes.
