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

Contains six active claw movement functions:
1. XYZservo         -Handles "active" (while game is live) X, Y and Z movement logic. All "active" motion is controlled via time-based waits. This allows concurrency between different directions (claw can move in X, Y, and Z directions at once) even though stepper motors step pins must necessarily wait between being assigned high and low to perform a full motion. Every other millisecond, a stepper motor's step pin is assigned to be 1 or not based on if it has received a signal to move in that direction. Then on the alternating millisecond, every stepper motor step pin is assigned to zero. If a signal has occurred to move in any combination of directions, there will be the required fluctuation in step pins for that combination. There is one stipulation to this rule:

X and Z direction movement is naturally very smooth. The game is rigged such that the claw travels in the Z direction on a string using one stepper motor, and in the X direction upon one bar also using one stepper motor. However, when the claw travels in the Y direction it has to move the entire X-direction bar, and must use two stepper motors. This introduces a large amount of vibration and is very harsh on the mechanical aspects of this project. In order to handle this issue, microstepping is used for the two y-direction stepper motors. This smooths the movement but slows down the speed in the y direction. To handle this, the y direction movement is handled every 400 microseconds instead of every millisecond. This direction uses a wait specifically, but is fast enough compared to the other directions that it is not noticeable. 

2. OpenClose        - Handles "active" claw rotation movement using a servo motor. This allows us to write to a specific angle. The current claw angle is calculated at each step, and then written to increment or decrement depending on whether or not the claw open/close buttons are pressed. Because the servo is handled slightly differently from the steppers, the functions are different. However, because a time-based activation is still used, the claw can be opened or closed at the same time the claw is moving in any X, Y, and/or Z direction
3. writeClawmS      - Handles non-active claw rotation and X, Y, Z motion in terms of number of milliseconds. A motion will still not be carried out if the claw is already at the limit in that direction.  Otherwise, the claw will move in the specified direction for the specified amount of time in milliseconds. Because the value that the claw rotation will write to is known, the rotation can be included in this function. This allows for motion directly writing the claw closed so it will hold onto the toy. This function is primarily used in the arcadeSequence function to grab the toy automatically. Time-based activation is still used because I think it is quite elegant.
4. writeClawBar     - Handles non-active claw X, Y, Z motion in terms of position relative to a specific set of edges. This function will run until the claw has hit all edges specified, and then will stop. This function is used primarily in the homingSequence function to return the claw to its zero positions above the toy exit zone. Because there is no Z negative edge, that direction is not an option. Because there is no feedback from the servo motor to tell us where it is currently located, two extra functions for writing the claw all the way to open and all the way to closed are used:
5. writeClawOpen    - Handles non-active claw to open position rotation movement.
6. writeClawClose   - Handles non-active claw to closed position rotation movement.![image](https://user-images.githubusercontent.com/90006904/143777396-8b093d22-6710-4a37-bb67-6b7f3818a666.png)


Contains one "future use" function:
1. recalibrate      -

# Claw_Machine_Constants.h FILE:

Contains all pin assignment constants for Arduino. Contains display setup information and constant display writes.
