#include <LiquidCrystal.h>

// Lupe Carlos || Brian Lee || Ryan Bates
// TechSpark Claw Machine
// 8/30/2021

//TODO: make it so if I click the green button again during game, does nothing or game ends
//TODO: add adafruit neopixel library and LED strips

//include libraries
#include <Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//declare servo motor using Servo library
Servo servoClaw;

//declare liquid crystal lcd
LiquidCrystal_I2C lcd(0x27, 20, 4);

//Special byte characters for loading bar lcd

byte one[] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000
};

byte two[] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000
};

byte three[] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100
};

byte four[] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110
};

byte five[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte bottom[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111
};

byte top[] = {
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

//pin assignment variables
int clawPin = 5;
int clawUp = 26;
int clawDown = 27;
int clawClose = 28;
int clawOpen = 29;

int clawStart = 30;
int servoRelay = 53;
int gameModePin = 16;

int xStepPin = 11;
int xDirPin = 10;
int y1StepPin = 9;
int y1DirPin = 8;
int y2StepPin = 7;
int y2DirPin = 6;
int zStepPin = 13;
int zDirPin = 12;
int stepperEnable = 52;

int xMinLmtSwtch = 15;
int xMaxLmtSwtch = 14;
int yMinLmtSwtch = 3;
int yMaxLmtSwtch = 2;
int zMaxLmtSwtch = 4;

int forwardJoystick = 23;
int backwardJoystick = 22;
int leftJoystick = 25;
int rightJoystick = 24;

//TODO: code review
int potPin = A12; //TODO: FIND ANALONG PIN

//loading bar variables
int barWidth = 20;

void pinAssign(){
  pinMode(xStepPin, OUTPUT);
  pinMode(xDirPin, OUTPUT);
  
  pinMode(y1StepPin, OUTPUT);
  pinMode(y1DirPin, OUTPUT);
  pinMode(y2StepPin, OUTPUT);
  pinMode(y2DirPin, OUTPUT);
  pinMode(zStepPin, OUTPUT);
  pinMode(zDirPin, OUTPUT);
  pinMode(stepperEnable, OUTPUT);
  
  pinMode(servoRelay, OUTPUT);

  pinMode(xMinLmtSwtch, INPUT_PULLUP);
  pinMode(xMaxLmtSwtch, INPUT_PULLUP);
  pinMode(yMinLmtSwtch, INPUT_PULLUP);
  pinMode(yMaxLmtSwtch, INPUT_PULLUP);
  pinMode(zMaxLmtSwtch, INPUT_PULLUP);

  pinMode(clawUp, INPUT_PULLUP);
  pinMode(clawDown, INPUT_PULLUP);
  pinMode(clawOpen, INPUT_PULLUP);
  pinMode(clawClose, INPUT_PULLUP);
  pinMode(forwardJoystick, INPUT_PULLUP);
  pinMode(backwardJoystick, INPUT_PULLUP);
  pinMode(leftJoystick, INPUT_PULLUP);
  pinMode(rightJoystick, INPUT_PULLUP);
  
  pinMode(gameModePin, INPUT_PULLUP);
  pinMode(clawStart, INPUT_PULLUP);

  pinMode(potPin, INPUT);

  servoClaw.attach(clawPin);
}

void lcdWriteDefault(){
  lcd.clear();
  lcd.setCursor(0,0);
  if (arcadeMode) { lcd.print("~~~~ARCADE MODE!~~~~"); }
  else {            lcd.print("~~~~FULL CONTROL!~~~"); }
  lcd.setCursor(0,1);
  lcd.print("*PRESS GREEN BUTTON*");
  lcd.setCursor(0,3);
  lcd.print("you have");
  lcd.setCursor(9,3);
  lcd.print(gameTimeS);
  lcd.setCursor(12,3);
  lcd.print("seconds");
}

void lcdSetup(){
  lcd.begin();
  lcd.createChar(1, one);
  lcd.createChar(2, two);
  lcd.createChar(3, three);
  lcd.createChar(4, four);
  lcd.createChar(5, five);
  lcd.createChar(6, top);
  lcd.createChar(7, bottom);
  lcd.backlight();
  lcdWriteDefault();
}
