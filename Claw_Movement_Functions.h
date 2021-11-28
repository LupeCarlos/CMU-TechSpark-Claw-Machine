// Lupe Carlos || Brian Lee || Ryan Bates
// TechSpark Claw Machine
// 8/30/2021

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
  
  bool dirWrite = ((currentMillis % 13) == 0);

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

    if (!prevFB) { 
      FBmillis = millis() + 1;
      Serial.println("saw start of move");
    }
    
    unsigned long FBdiff = millis() - FBmillis;
    int modFB = 2 + 1000/(400 + FBdiff);

    digitalWrite(y1StepPin, ((currentMillis % modFB) == 1));
    digitalWrite(y2StepPin, ((currentMillis % modFB) == 1));
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
void writeClawmS(int mS, bool 
, bool left, bool forward, bool backward, bool up, bool down, bool op, bool cl){
  bool LR = (right && digitalRead(xMaxLmtSwtch))  || (left && digitalRead(xMinLmtSwtch));
  bool FB = (forward && digitalRead(yMaxLmtSwtch)) || (backward && digitalRead(yMinLmtSwtch));
  bool UD = (up && digitalRead(zMaxLmtSwtch)) || down;
  bool OC = op || cl;
  
  digitalWrite(xDirPin, left);
  digitalWrite(y1DirPin, forward);
  digitalWrite(y2DirPin, backward);
  digitalWrite(zDirPin, up);
  int ope = op ? 120 : 0;

  digitalWrite(servoRelay, HIGH);
  unsigned long millisCountmS = millis();
  unsigned long currentMillismS; 
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

  while ((right && digitalRead(xMaxLmtSwtch)) || (left && digitalRead(xMinLmtSwtch)) || 
         (forward && digitalRead(yMaxLmtSwtch)) || (backward && digitalRead(yMinLmtSwtch)) ||
         (up && digitalRead(zMaxLmtSwtch))){
    
    if ((right && digitalRead(xMaxLmtSwtch)) || (left && digitalRead(xMinLmtSwtch))) {
      digitalWrite(xStepPin, HIGH);
    }

    if ((forward && digitalRead(yMaxLmtSwtch)) || (backward && digitalRead(yMinLmtSwtch))) {
      digitalWrite(y1StepPin, HIGH); digitalWrite(y2StepPin, HIGH); 
    }

    if (up && digitalRead(zMaxLmtSwtch)) {
      digitalWrite(zStepPin, HIGH);
    }

    delay(1);
    digitalWrite(xStepPin, LOW);
    digitalWrite(y1StepPin, LOW);
    digitalWrite(y2StepPin, LOW);
    digitalWrite(zStepPin, LOW);
    delay(1);
  }
}

void writeClawOpen(){
  Serial.write("saw claw open cmd\n");
  writeClawmS(1000, 0, 0, 0, 0, 0, 0, 1, 0); //open
}

void writeClawClose(){
  Serial.write("saw claw close cmd\n");
  writeClawmS(1000, 0, 0, 0, 0, 0, 0, 0, 1); //close
}

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

  delay(30000); 
}
