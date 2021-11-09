// Lupe Carlos || Brian Lee || Ryan Bates
// TechSpark Claw Machine
// 8/30/2021

//TODO: make it so if I click the green button again during game, does nothing or game ends
//TODO: add adafruit neopixel library and LED strips

//initialize loop variables

//game control variables
bool gameOn = 0;
bool start = 0;
int startBefore = 0;

//claw initialization variables
int clawCurrentValue = 1;

//gameTime control variables
int potentiometer = 512;
int potentiometer_prev = 512;

void arcadeSequence(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hey Alexa,");
  lcd.setCursor(0,1);
  lcd.print("play 'The Claw'");
  lcd.setCursor(0,2);
  lcd.print("by 'The Aliens'");
  lcd.setCursor(0,3);
  lcd.print("on Spotify");

  writeClawOpen();
  writeClawmS(4200, 0, 0, 0, 0, 0, 1, 0, 0); //down
  writeClawClose();
  writeClawmS(4200, 0, 0, 0, 0, 1, 0, 0, 1); //up and close
}

void homingSequence(){
  lcd.setCursor(0,0);
  lcd.print("GAME OVER!! GAME OVER!! GAME OVER!! GAME OVER!! GAME OVER!! GAME OVER!! GAME OVE");

  writeClawBar(0, 1, 0, 1, 1); //backward, left, and up to bar

  delay(1500);

  writeClawmS(400, 0, 0, 0, 0, 0, 1, 0, 0); //down a bit

  writeClawOpen();
  writeClawClose();
  writeClawBar(0, 0, 0, 0, 1); //backward, left, and up to bar

  
}

void gameOnLcdUpdate(unsigned long currentMillis){
    //want it to come every one (width*5)-th of the time
    if ((currentMillis % (10*gameTimeS)) == 0){
      lcd.setCursor((currentMillis/(50*gameTimeS)),2);
      lcd.write(1 + (currentMillis/(10*gameTimeS) % 5));
    }
    
    if ((currentMillis % 1000) == 0){
      lcd.setCursor(16,0);
      lcd.print("  ");
      lcd.setCursor(16,0);
      lcd.print(gameTimeS - currentMillis/1000);
    }
}

void gameInitialize(){
  
    millisCount = millis();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("game counter = ");
    lcd.setCursor(16,0);
    lcd.print(gameTimeS);
    
    for (int i = 0; i < barWidth; i++){
      lcd.setCursor(i,1);
      lcd.write(7);
    }

    for (int j = 0; j < barWidth; j++){
      lcd.setCursor(j,3);
      lcd.write(6);
    }
  
    digitalWrite(stepperEnable, LOW);
  
}

bool countDownandEnd(bool gameOn, bool justStarted){
  
  currentMillis = millis() - millisCount;

  if (gameOn) {
    if ((currentMillis > 1000) && justStarted) { currentMillis = gameTimeS*1000; }
    gameOnLcdUpdate(currentMillis);

    if (currentMillis/1000 >= gameTimeS){ 
  
      if (arcadeMode) {arcadeSequence();}
      homingSequence();
      digitalWrite(stepperEnable, HIGH);
      digitalWrite(servoRelay, LOW);

      lcdWriteDefault();
      gameOn = 0;
    }
  }

  return gameOn;
}

bool gameRun(bool gameOn, bool start, bool startBefore){

  bool justStarted = start && !startBefore;
  if (!gameOn && justStarted){
    gameOn = 1;
    gameInitialize();
  }
  gameOn = countDownandEnd(gameOn, justStarted);

  return gameOn;
  
}
