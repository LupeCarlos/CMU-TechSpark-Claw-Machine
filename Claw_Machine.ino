// Lupe Carlos || Brian Lee || Ryan Bates
// TechSpark Claw Machine
// 8/30/2021
//
// Lengthy description in README

//game assignment variables
bool arcadeMode; //1: arcade mode, 0: full control
int gameTimeS = 30; //length of game in seconds

//Claw open/close speed is based off of "what feels right"
int openSpeed = 50; //how fast claw will open/close

//reads should be mostly in this main file,
//writes should be mostly in included files:
#include "Claw_Machine_Constants.h"
#include "Claw_Movement_Functions.h"
#include "Claw_Game_Control_Functions.h"

void setup() {

  // initialize the serial port
  Serial.begin(9600);

  //set up pin assignments
  pinAssign();

  //turn off stepper motors at start of time
  digitalWrite(stepperEnable, HIGH);

  //read game mode only at start;
  arcadeMode = digitalRead(gameModePin);
  
  //set up LCD screen with start values for game
  lcdSetup();

}

void loop() {

  //START GAME SEQUENCE
  startBefore = start;
  start = !digitalRead(clawStart);
  gameOn = gameRun(gameOn, start, startBefore);
  
  if (gameOn) {

    //STEPPER MOTORS
  
    //JOYSTICK DIRECTIONS
    right = !digitalRead(rightJoystick) && digitalRead(xMaxLmtSwtch);
    left = !digitalRead(leftJoystick) && digitalRead(xMinLmtSwtch);
    forward = !digitalRead(forwardJoystick) && digitalRead(yMaxLmtSwtch);
    backward = !digitalRead(backwardJoystick) && digitalRead(yMinLmtSwtch);

    if (!arcadeMode) {
      
      //CLAW UP OR DOWN
      up = !digitalRead(clawUp) && digitalRead(zMaxLmtSwtch);
      down = !digitalRead(clawDown);

      //SERVO MOTOR

      //CLAW OPEN OR CLOSE
      opening = (!digitalRead(clawOpen) && (clawCurrentValue < 120000/openSpeed));
      closing = (!digitalRead(clawClose) && (clawCurrentValue > 0));
    
      if (opening || closing) {
        clawCurrentValue = openClose(opening, closing, clawCurrentValue);
      }
    }

    XYZservo(right, left, forward, backward, up, down);
    
  } else {
    
    //GAME TIME CONTROL USING POTENTIOMETER
    potentiometer = analogRead(potPin);

    //if the potentiometer has changed by more than 8 values
    //(to get rid of noise or small bumps)
    if (abs(potentiometer - potentiometer_prev) > 8) {

      //set gameTime actively based off of 
      gameTimeS = 30 + ((512 - potentiometer))/17;

      //actively print game time on display
      lcd.setCursor(9,3);
      lcd.print("   ");
      lcd.setCursor(9,3);
      lcd.print(gameTimeS);

      //save old potentiometer value so we know how much to change
      potentiometer_prev = potentiometer;
    }
  }
}

 
