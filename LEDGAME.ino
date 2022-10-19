#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <stdio.h> 
#include <time.h> 
LiquidCrystal_I2C lcd(0x27,16,2);  // lcd display setup
int buttInit[4] = {4, 7, 10, 12} ;
int ledInit[4] = {3, 9, 5, 2} ;
int buttState[4];
int score = 0;
int gameState = 1;
#define LOGO (1)
#define GAME_SET_N_START (2)
#define GAME_RUNNING (3)
#define GAME_SCORE (4)
#define GAME_END (5)
#define BUTTONS_QUANTITY (4)
bool buttPressStatus = false;

class Timer
{
  public:
  unsigned long  newTimeValue;
  unsigned long  limitValue;
  unsigned long  oldTimeValue;
  unsigned long  timeleft;

  void updateTimer()
  {
    newTimeValue = millis();
  }

  void specialDelayTimer()
  {
    while (isTimerRunning())
    {
      if(digitalRead(buttInit[0]) == HIGH)
      { 
        buttState[0] = 1; 
        break;
      }   
        else if (!isTimerRunning())
        {
          break;
        }
    }
  }

  void delayTimer()
  {
    while (isTimerRunning())
    {
      if (!isTimerRunning())
      {
        break;
      }
    }
  }
  
  void setTimer(unsigned long timeToSet)
  {
    oldTimeValue = millis();
    limitValue = timeToSet;
  }
  
  bool isTimerRunning()
  {
    newTimeValue = millis();      
    return ((newTimeValue - oldTimeValue) <= limitValue);
  }

  int timeLeft()
  {
    timeleft = limitValue - (newTimeValue - oldTimeValue);
    return timeleft;
  }

};

Timer timer; 
Timer ledTimer;
Timer waitTimer;
Timer scoreTimer;
Timer lcdTimer;
Timer logoTimer;

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

 //turn on led
int ledOn(int num)
{
  digitalWrite(ledInit[num], HIGH);   
}

//turn off all leds
void ledOff(int num)
{
  digitalWrite(ledInit[num], LOW);
}  

//butt buffer 
int checkButtPress()
{  
  for (int i = 1; i < BUTTONS_QUANTITY; i++)
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
 if (buttState[num] == 1 && buttPressStatus)
  {
    score += 1;
    lcd.clear();
    lcd.print("    CORRECT!");
  }
 else if (buttState[num] == 0 && buttPressStatus)
  {
    lcd.clear();
    lcd.print("   INCORRECT(");
  }
 else if (!ledTimer.isTimerRunning() && !buttPressStatus)
  {
    lcd.clear();
    lcd.print("NO BUTTON PRESS(");
  }
}

//clearing butt bufer
void clearButt()
{
  buttState[0] = 1; 
  buttPressStatus = false;   
  for (int i = 1; i < BUTTONS_QUANTITY; i++)
   {
     buttState[i] = 0;
   }
}

void userSetGameTime()
{
  lcd.clear();
  lcd.print(" set game time!");
  lcd.setCursor(0, 1);
  lcd.print("1=10, 2=20, 3=30");
  while( !buttPressStatus )
  {
    checkButtPress();
    for (unsigned long i = 1, t = 10000; i < BUTTONS_QUANTITY; i++)
    {
      if (buttState[i] == 1)
      {  
        lcd.clear();
        lcd.print("  setted time:");
        lcd.setCursor(7, 1);
        lcd.print(i);
        lcd.setCursor(8, 1);
        lcd.print("0!");
        lcdTimer.setTimer(2000);
        lcdTimer.delayTimer();
        countdown();
        timer.setTimer(t*i); //set how long game will last 
      }
    } 
  }
}

void countdown()
{
  for (int i = 5; i > 0; i--)
  {
    lcd.clear();
    lcd.print(" game start in:");
    lcd.setCursor(8, 1);
    lcd.print(i);
    lcdTimer.setTimer(1000);
    lcdTimer.delayTimer();
  }        
  lcd.clear();
  lcd.print("  game started ");
}

void printScore()
{
  lcd.setCursor(0, 1);
  lcd.print("your score is:");
  lcd.print(score);
}

void printGameOverScore()
{
  lcd.clear();
  lcd.print("   GAME OVER!");
  lcd.setCursor(0, 1);
  lcd.print(" final score:");
  lcd.print(score);
}

void printPressStartButton()
{
  lcd.clear(); 
  lcd.print("LED BUTTON GAME!");
  while (buttState[0] == 0) 
  {
     if(digitalRead(buttInit[0]) == HIGH)
    { 
      buttState[0] = 1; 
    }   
    lcd.setCursor(0, 1);
    lcd.print("press start butt");
    logoTimer.setTimer(1000);
    logoTimer.specialDelayTimer();
    lcd.clear(); 
    lcd.print("LED BUTTON GAME!");
    logoTimer.setTimer(1000);
    logoTimer.specialDelayTimer();
  }
}


void setup() 
{  
  lcd.init();                     
  lcd.backlight();
  Serial.begin(9600); 
  randomSeed(analogRead(0));
  srand(time(NULL));
     
  for (int s = 0; s < BUTTONS_QUANTITY; s++) //here BUTTONS_QUANTITY means butt and leds quantity (its always same)
  {
    pinMode(ledInit[s], OUTPUT);
    pinMode(buttInit[s], INPUT);
  } 
}

void loop() 
{ 
  int b = randomGenLed();
  int t = randomGenTime();
  
  
  switch (gameState) 
  {

   case LOGO:
    printPressStartButton();
    clearButt();
    userSetGameTime();
    gameState = GAME_RUNNING;
           
  case GAME_RUNNING: // the game
    timer.timeLeft();
    clearButt();       
    timer.updateTimer(); // updating and init timer value
    waitTimer.setTimer(t);
    waitTimer.delayTimer();
    ledTimer.updateTimer();
    if (timer.timeLeft() <= 999)
    {
      t = timer.timeLeft();    
      gameState = GAME_SCORE;     
    }
      else
      {
        ledOn(b); //turn on led
        ledTimer.setTimer(t); //setting time for leds 
        buttPressStatus = false;
        while (ledTimer.isTimerRunning())
        {   
          checkButtPress();
          if (buttPressStatus)
          {
           break;
          }
        } 
        checkResult(b); //      
        clearButt(); //clearing butt bufer
        ledOff(b); //turn off led
        printScore();
      }
   
    break;     

  case GAME_SCORE:
    scoreTimer.setTimer(5000);
    printGameOverScore(); 
    scoreTimer.delayTimer();
    gameState = GAME_END;
    break;
    
  case GAME_END: //game over        
   gameState = LOGO;
   score = 0;
   buttState[0] = 0;
   break;
            
  default:
    break;
  }
}
