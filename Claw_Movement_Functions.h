// Lupe Carlos || Brian Lee || Ryan Bates
// TechSpark Claw Machine
// 8/30/2021
//
// Lengthy description in README

//initialize loop variables

//claw motion variables
bool right = 0;
bool left = 0;
bool forward = 0;
bool backward = 0;
bool up = 0;
bool down = 0;
bool opening = 0;
bool closing = 0;

//Y-dir speedup variables
bool prevFB = 0;
unsigned long FBmillis = 0;

//initialize global variables

//global timing variables
unsigned long millisCount = 0;
unsigned long currentMillis = 0;
unsigned long microsCount = 0;
unsigned long currentMicros = 0;

//global positioning system ;)
unsigned long zUpSteps = 0;
unsigned long xDistanceSteps = 1425;
unsigned long yDistanceSteps = 1082;
unsigned long zDistanceSteps = 0;
unsigned long xStepPosition = 0;
unsigned long yStepPosition = 0;
unsigned long zStepPosition = 0;

//Continuous-time Claw direction movement (while game is running)
void XYZservo(bool right, bool left, bool forward, bool backward, bool up, bool down){

  bool LR = left || right;
  bool FB = forward || backward;
  bool UD = up || down;
  
  bool dirWrite = ((currentMillis % 15) == 0);

  if (LR) {
    if (dirWrite) {
      digitalWrite(xDirPin, left);
    }
    
    digitalWrite(xStepPin, ((currentMillis % 2) == 1));
  }
 
  if (FB) { 
    if (dirWrite) {
      digitalWrite(y1DirPin, forward);
      digitalWrite(y2DirPin, backward);
    }
    
    digitalWrite(y1StepPin, 1);
    digitalWrite(y2StepPin, 1);
    delayMicroseconds(400);
    digitalWrite(y1StepPin, 0);
    digitalWrite(y2StepPin, 0);
    delayMicroseconds(400);
    
  }
  
  if (UD) {
    if (dirWrite) {
      digitalWrite(zDirPin, up);
    }

    digitalWrite(zStepPin, ((currentMillis % 2) == 1));
  }
}

//Continuous-time Claw open/close movement (while game is running)
int openClose(bool opening, bool closing, int clawCurValue){
  digitalWrite(servoRelay, HIGH);
  if (closing) {clawCurValue = clawCurValue - 1; }
  if (opening) {clawCurValue = clawCurValue + 1; }
  servoClaw.write(clawCurValue/(1000/openSpeed));
  return clawCurValue;
}

//Specific-time direction and open/close movement (for directed movement sequences)
void writeClawmS(int mS, bool right, bool left, bool forward, bool backward, bool up, bool down, bool op, bool cl){
  bool LR = (right && digitalRead(xMaxLmtSwtch))  || (left && digitalRead(xMinLmtSwtch));
  bool FB = (forward && digitalRead(yMaxLmtSwtch)) || (backward && digitalRead(yMinLmtSwtch));
  bool UD = (up && digitalRead(zMaxLmtSwtch)) || down;
  bool OC = op || cl;
  
  digitalWrite(xDirPin, left);
  digitalWrite(y1DirPin, forward);
  digitalWrite(y2DirPin, backward);
  digitalWrite(zDirPin, up);
  int ope = op ? 120 : 0;

  digitalWrite(stepperEnable, LOW);
  digitalWrite(servoRelay, HIGH);
  unsigned long millisCountmS = millis();
  unsigned long currentMillismS = 0; 
  while (currentMillismS < mS){
    currentMillismS = millis() - millisCountmS;
    
    if (LR) {
      digitalWrite(xStepPin, ((currentMillismS % 2) == 1));
      if (right) { xStepPosition += (currentMillismS % 2); }
      else {       xStepPosition -= (currentMillismS % 2); }
    }
    
    if (FB) {
      digitalWrite(y1StepPin, ((currentMillismS % 2) == 1));
      digitalWrite(y2StepPin, ((currentMillismS % 2) == 1));
      if (forward) { yStepPosition += (currentMillismS % 2); }
      else {         yStepPosition -= (currentMillismS % 2); }
    }
    
    if (UD) {
      digitalWrite(zStepPin, ((currentMillismS % 2) == 1));
      if (up) { zStepPosition += (currentMillismS % 2); }
      else {    zStepPosition -= (currentMillismS % 2); }
    }
    
    if (OC) {servoClaw.write(ope); }
  }
  
  digitalWrite(stepperEnable, HIGH);
}

//Move-to-bar direction and open/close movement (for directed movement sequences)
void writeClawBar(bool right, bool left, bool forward, bool backward, bool up){
  
  digitalWrite(xDirPin, left);
  digitalWrite(y1DirPin, forward);
  digitalWrite(y2DirPin, backward);
  digitalWrite(zDirPin, up);

  if (right)         { xStepPosition = xDistanceSteps; }
  else if (left)     { xStepPosition = 0; }
  if (forward)       { yStepPosition = yStepPosition; }
  else if (backward) { yStepPosition = 0; }
  if (up)            { zStepPosition = 0; }

  digitalWrite(stepperEnable, LOW);

  while ((right && digitalRead(xMaxLmtSwtch)) || (left && digitalRead(xMinLmtSwtch)) || 
         (forward && digitalRead(yMaxLmtSwtch)) || (backward && digitalRead(yMinLmtSwtch)) ||
         (up && digitalRead(zMaxLmtSwtch))){

    unsigned long WBCmillis = millis();
    
    if ((right && digitalRead(xMaxLmtSwtch)) || (left && digitalRead(xMinLmtSwtch))) {
      digitalWrite(xStepPin, ((WBCmillis % 2) == 1));
    }

    if ((forward && digitalRead(yMaxLmtSwtch)) || (backward && digitalRead(yMinLmtSwtch))) {
      digitalWrite(y1StepPin, HIGH); digitalWrite(y2StepPin, HIGH); 
      delayMicroseconds(400);
      digitalWrite(y1StepPin, LOW);
      digitalWrite(y2StepPin, LOW);
      delayMicroseconds(400);
    }

    if (up && digitalRead(zMaxLmtSwtch)) {
      digitalWrite(zStepPin, ((WBCmillis % 2) == 1));
    }
    
  }
  
  digitalWrite(stepperEnable, HIGH);
}

//write claw to fully open position
void writeClawOpen(){
  writeClawmS(1000, 0, 0, 0, 0, 0, 0, 1, 0); //open
}

//write claw to fully closed position
void writeClawClose(){
  writeClawmS(1000, 0, 0, 0, 0, 0, 0, 0, 1); //close
}

/*
//calculte size of claw area in stepper motor steps
void recalibrate(){
  
  unsigned long xRightSteps = 0;
  unsigned long xLeftSteps = 0;
  unsigned long yForwardSteps = 0;
  unsigned long yBackwardSteps = 0;
  zDistanceSteps = 0;
  
  digitalWrite(stepperEnable, LOW);

  zDistanceSteps = millis();
  writeClawBar(0,0,0,0,1); //up
  zDistanceSteps = millis() - zDistanceSteps;
  
  writeClawBar(1,0,0,0,0); //right

  xLeftSteps = millis();
  writeClawBar(0,1,0,0,0); //left
  xLeftSteps = millis() - xLeftSteps;

  xRightSteps = millis();
  writeClawBar(1,0,0,0,0); //right
  xRightSteps = millis() - xLeftSteps;

  writeClawBar(0,0,0,1,0); //backward

  yForwardSteps = millis();
  writeClawBar(0,0,1,0,0); //forward
  yForwardSteps = millis() - yForwardSteps;

  yBackwardSteps = millis();
  writeClawBar(0,0,0,1,0); //backward
  yBackwardSteps = millis() - yBackwardSteps;
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(xRightSteps);
  lcd.setCursor(0,1);
  lcd.print(xLeftSteps);
  lcd.setCursor(0,2);
  lcd.print(yForwardSteps);
  lcd.setCursor(0,3);
  lcd.print(yBackwardSteps);

  xDistanceSteps = (xRightSteps + xLeftSteps)/4;
  yDistanceSteps = (yForwardSteps + yBackwardSteps)/4;
  zDistanceSteps = zDistanceSteps/2;
  
  digitalWrite(stepperEnable, HIGH);
}
*/
