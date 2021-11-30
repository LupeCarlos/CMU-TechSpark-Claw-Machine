// Lupe Carlos || Brian Lee || Ryan Bates
// TechSpark Claw Machine
// 8/30/2021
//
// Lengthy description in README

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


//perform typical arcade claw game sequence
void arcadeSequence(){

  //Display a small joke
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Hey Alexa,");
  lcd.setCursor(0,1);
  lcd.print("play 'The Claw'");
  lcd.setCursor(0,2);
  lcd.print("by 'The Aliens'");
  lcd.setCursor(0,3);
  lcd.print("on Spotify");

  //Retreive toy (on success)
  writeClawOpen();
  writeClawmS(4200, 0, 0, 0, 0, 0, 1, 0, 0); //down
  writeClawClose();
  writeClawmS(4200, 0, 0, 0, 0, 1, 0, 0, 1); //up and close
}

//return to zero position
void homingSequence(){
  lcd.setCursor(0,0);
  lcd.print("GAME OVER!! GAME OVER!! GAME OVER!! GAME OVER!! GAME OVER!! GAME OVER!! GAME OVE");

  //return claw to home base above toy exit zone
  writeClawBar(0, 1, 0, 1, 1); //backward, left, and up to bar

  //wait for claw to stop swinging
  delay(1500);

  //detangle claw from any awkward positioning
  writeClawmS(400, 0, 0, 0, 0, 0, 1, 0, 0); //down a bit

  //release toy
  writeClawOpen();
  writeClawClose();

  //return claw to zero position
  writeClawBar(0, 0, 0, 0, 1);
  
}

//update LCD during game time
void gameOnLcdUpdate(unsigned long currentMillis){

    //every second, update the amount of time left
    if ((currentMillis % 1000) == 0){
      lcd.setCursor(16,0);
      lcd.print("  ");
      lcd.setCursor(16,0);
      lcd.print(gameTimeS - currentMillis/1000);
    }
    
}

//initialize game once start button has been pressed
void gameInitialize(){

    //begin counting the milliseconds the game has been on
    millisCount = millis();

    //reset display for counting
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("game counter = ");
    lcd.setCursor(16,0);
    lcd.print(gameTimeS);

    //setup loading bar
    lcd.setCursor(0,2);
    lcd.print("Press Green Button");
    lcd.setCursor(0,3);
    lcd.print("Again to End Game");
  
    digitalWrite(stepperEnable, LOW);
  
}

//count down the clock, and end game when over
bool countDownandEnd(bool gameOn, bool justStarted){

  //keep active track of current time spent in active game mode
  currentMillis = millis() - millisCount;

  //if we are in game mode
  if (gameOn) {

    //if more than a second has passed and someone presses the start button
    //again, end the game
    if ((currentMillis > 5000) && justStarted) { currentMillis = gameTimeS*1000; }

    //update display with active game time
    gameOnLcdUpdate(currentMillis);

    //if we have reached the end of the allotted time
    if (currentMillis/1000 >= gameTimeS){ 

      //if we are in arcade mode, do arcade sequence
      if (arcadeMode) {
        arcadeSequence();
      }

      //return the claw to zero position and turn off all motors
      homingSequence();
      digitalWrite(stepperEnable, HIGH);
      digitalWrite(servoRelay, LOW);

      //return the display to game off mode
      lcdWriteDefault();

      //turn game off
      gameOn = 0;
    }
  }

  return gameOn;
}

//wrapper function to start the game
bool gameRun(bool gameOn, bool start, bool startBefore){

  //if the button wasn't pressed but is now, turn game on
  bool justStarted = start && !startBefore;
  if (!gameOn && justStarted){
    gameOn = 1;

    //initialize the game
    gameInitialize();
  }

  //call countdown funciton
  gameOn = countDownandEnd(gameOn, justStarted);

  return gameOn;
  
}
