
#include <stdio.h> 
int buttInit[4] = {4, 7, 10, 12} ;
int ledInit[4] = {3, 9, 5, 2} ;
int buttState[4];
int score = 0;
int gameState = 1;
#define GAME_SET_N_START (1)
#define GAME_RUNNING (2)
#define GAME_SCORE (3)
#define GAME_END (4)
bool buttPressStatus = false;

class Timer
{
  public:
  unsigned long  newTimeValue;
  unsigned long  limitValue;
  unsigned long  oldTimeValue;

  void updateTimer()
  {
    newTimeValue = millis();
  }
  
  void setTimer(unsigned long timeToSet)
  {
    oldTimeValue = millis();
    limitValue = timeToSet;
  }
  
  bool isTimerRunning()
  {
    newTimeValue = millis();
        
    if ( (newTimeValue - oldTimeValue) <= limitValue)
      return true;
    else 
      return false;
  }

};

Timer timer; 
Timer ledTimer;
Timer waitTimer;
Timer scoreTimer;

//generating random number for led
int randomGenLed()
{
  int num = rand() % 3 + 1; 
  return num; 
}

//generating random time for led
int randomGenTime()
{
  int randomtime = (rand() % 9 + 3) * 100; 
  return randomtime;
}

 //turn on random led
int ledOn(int num)
{
  digitalWrite(ledInit[num], HIGH);   
}

//turn off all leds
void ledOff(int num)
{
  digitalWrite(ledInit[num], LOW);
}  

//буфер с нажатыми кнопками
int checkButtPress()
{  
  for (int i = 1; i < 4; i++)
  {
    if (digitalRead(buttInit[i]) == HIGH)
    {
      buttState[i] = 1;
      buttPressStatus = true;
    }
  }  
}

int checkResult(int num)
{ 
 if (buttState[num] == 1 && buttPressStatus == true)
  {
    score += 1;
    Serial.print("                                                                  CORRECT! \n");
  }
 else if (buttState[num] == 0 && buttPressStatus == true)
  {
    Serial.print("                                                                 INCORRECT( \n");
  }
 else if (ledTimer.isTimerRunning() == false && buttPressStatus == false)
  {
    Serial.print("                                                          NO BUTTON PRESS DETECTED \n");
  }
}

//clearing butt bufer
void clearButt()
{
  buttState[0] = 1; 
     
  for (int i = 1; i < 4; i++)
   {
     buttState[i] = 0;
   }
}

 //waiting for start button
void startButtonWait()
{
  while (buttState[0] == 0) 
  {
    if(digitalRead(buttInit[0]) == HIGH)
    { 
      buttState[0] = 1; 
    }
  }
}

void printScore()
{
  Serial.print("                                                              your score is ");
  Serial.print(score);
  Serial.print("\n \n");
}

void printGameOverScore()
{
  Serial.print("\n\n\n\n\n\n\n\n\n");
  Serial.print("                                                                 GAME OVER\n");
  Serial.print("                                                          your final score is ");
  Serial.print(score);
  Serial.print(" !!! \n\n\n\n\n\n\n\n\n");
}

void printPressStartButton()
{
  Serial.print("\n\n\n\n\n");
  Serial.print("                                                        Please press Start Button!!!\n\n");
  Serial.print("\n\n\n\n\n"); 
}

void setup() 
{  
  Serial.begin(9600); 
  randomSeed(analogRead(0));
     
  for (int s = 0; s < 4; s++)
  {
    pinMode(ledInit[s], OUTPUT);
    pinMode(buttInit[s], INPUT);
  } 
}

void loop() 
{ 
  int b = randomGenLed();
  int t = randomGenTime();
  unsigned long timeleft = timer.limitValue - (timer.newTimeValue - timer.oldTimeValue);
  
  switch (gameState) 
  {
            
   case GAME_SET_N_START: // the game start

    printPressStartButton();
    startButtonWait(); //waiting for start button
    timer.updateTimer(); // updating and init timer value
    timer.setTimer(15000); //set how long game will last 
    gameState = GAME_RUNNING;
    break;
        
  case GAME_RUNNING: // the game
            
    timer.updateTimer(); // updating and init timer value
    if ((timeleft) <= 999)
    {
      t = timeleft;    
      gameState = GAME_SCORE;     
    }
    ledOn(b); //turn on led
    ledTimer.setTimer(t); //setting time for leds 
    buttPressStatus = false;
    while (ledTimer.isTimerRunning() == true )
    {   
      checkButtPress();
      if (buttPressStatus == true)
      {
        break;
      }
    } 
    checkResult(b); //      
    clearButt(); //clearing butt bufer
    ledOff(b); //turn off led
    printScore();
    waitTimer.setTimer(t);
    while (waitTimer.isTimerRunning() == true )
    {
      if (waitTimer.isTimerRunning() == false )
      {
        break;
      }
    }
    break;     

  case GAME_SCORE:
    scoreTimer.setTimer(5000);
    printGameOverScore(); 
    while (scoreTimer.isTimerRunning() == true )
    {
      if (scoreTimer.isTimerRunning() == false )
      {
        break;
      }
    }
    gameState = GAME_END;
    break;
    
  case GAME_END: //game over        
   gameState = GAME_SET_N_START;
   score = 0;
   buttState[0] = 0;
   break;
            
  default:
    break;
  }
}
