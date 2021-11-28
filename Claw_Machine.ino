// Lupe Carlos || Brian Lee || Ryan Bates
// TechSpark Claw Machine
// 8/30/2021

//game assignment variables
bool arcadeMode; //1: arcade mode, 0: full control
int gameTimeS = 30;//length of game in seconds

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

  //calibrate machine for xyz coordinatess
  //recalibrate();
    

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
    prevFB = forward || backward;
    
  } else {
    potentiometer = analogRead(potPin);
    if (abs(potentiometer - potentiometer_prev) > 8) {
      gameTimeS = 30 + ((512 - potentiometer))/17;
    
      lcd.setCursor(9,3);
      lcd.print("   ");
      lcd.setCursor(9,3);
      lcd.print(gameTimeS);
      potentiometer_prev = potentiometer;
    }
  }
}

 
