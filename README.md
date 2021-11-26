# CMU-TechSpark-Claw-Machine
Carnegie Mellon University TechSpark's very own claw machine. (well, this is the code for that.)

This repo consists of four files:
1. Claw_Machine.ino                 - Contains setup and loop arduino functions, includes below three files with constants and functions
2. Claw_Game_Control_Functions.h    - Contains functions that control game setup, timing, and ending 
4. Claw_Movement_Functions.h        - Contains functions that control claw movement
3. Claw_Machine_Constants.h         - Contains constants used by functions

# Claw_Machine.ino FILE:

The overall structure is contained in the top level file Claw_Machine.ino. Most of the inputs from the claw buttons are handled in this file,
and are distributed to different game functions based on what they are used for. Ie. Start button's input is sent to functions in
Claw_Game_Control_Functions.h, and direction inputs are sent to functions in Claw_Movement_functions.h. 

This claw system relies on serveral different sets of inputs:

Claw Movment inputs:
1. X, Y, Z positive and negative values (Z controlled by two separate buttons, X and Y controlled by joystick)
2. X, Y, minimum/maximum direction detection. Z minimum direction detection. (so directions aren't driven outside of bounds)
3. Claw Open/Close motion driving (Controlled by two separate buttons)

Directional information is given only to movement functions as an and: the claw movement direction must be high AND the claw must not be
hitting the edge in the direction the user is attempting to go.

Claw Control inputs:
1. Game Start button -- Starts active game mode. if pressed again after 5 seconds since game has started, will automatically end game
2. Game mode -- switch between FULL CONTROL mode and ARCADE mode. 
    -In FULL CONTROL mode, players can control all directions and claw opening and closing
    -In ARCADE MODE, players can only control X and Y directions. Upon game end, Claw will open and descend, close and rise, like a typical claw arcade game.
4. Potentiometer -- for active game length control

# Claw_Game_Control_Functions.h FILE:

Contains six game control functions:
1. gameRun            -Handles game start logic and calls gameInitialize and countDownandEnd. Returns returns game on status to be handled in Claw_Machine.ino, and is only function called in Claw_Machine.ino
2. CountDownandEnd    -Handles countdown of game and calls gameOnLcdUpdate. Handles end of game logic and calls arcadeSequence if in arcade mode
3. gameOnLcdUpdate    -Handles active LCD updating of game time.
4. arcadeMode         -Handles static (unchanging) arcade movement sequence of claw open, drop claw, claw close, return to 0 z position
5. homingSequence     -Handles static (unchanging) homing movement sequence where claw is returned to 0 X, Y, and Z positions, and claw opens and then closes
6. gameInitialize     -Initialize game mode on display, and set stepper motors to power-on state

# Claw_Movement_Functions.h FILE:

Contains global timing variables, global position variables, and global direction motion variables.

Contains six active claw movement variables:
1. XYZservo         -
2. OpenClose        -
3. writeClawmS      -
4. writeClawBar     -
5. writeClawOpen    -
6. writeClawClose   -

Contains one "future use" function:
1. recalibrate      -

# Claw_Machine_Constants.h FILE:

Contains all pin assignment constants for Arduino. Contains display setup information and constant display writes.
