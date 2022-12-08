#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <time.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // lcd display setup
int buttInit[4] = {4, 7, 10, 12} ;
int ledInit[4] = {3, 9, 5, 2} ;
#define LOGO (1)
#define GAME_SET_N_START (2)
#define GAME_RUNNING (3)
#define GAME_SCORE (4)
#define GAME_END (5)
#define BUTTONS_QUANTITY (4)

class Timer
{
  public:
    
    int oldValue()
    {
      unsigned long oldTimeValue = millis();
      return oldTimeValue;
    }

    int newValue()
    {
      unsigned long newTimeValue = millis();
      return newTimeValue;
    }

    bool calculateIsItRunning(unsigned long oldTimeValue, unsigned long limitValue)
    {
      unsigned long newTime = newValue();
      return (( newValue() - oldTimeValue) <= limitValue);
    }
    
    bool isRunning(unsigned long limitValue)
    {
      unsigned long oldTime = oldValue();
      return calculateIsItRunning(oldTime, limitValue);
    }

    int calculateTimeLeft(unsigned long oldTimeValue, unsigned long limitValue)
    {
      unsigned long newTime = newValue();
      return limitValue - (newTime - oldTimeValue);
    }

    int timeLeft(unsigned long limitValue)
    {
      unsigned long oldTime = oldValue();
      return calculateTimeLeft(oldTime, limitValue);
    }

        void specialDelayTimer(int *buttState)
    {
      while (isRunning(1000))
      {
        if (digitalRead(buttInit[0]) == HIGH)
        {
          buttState[0] = 1;
          break;
        }
        else if (!isRunning(1000))
        {
          break;
        }
      }
    }

    void dlay()
    {
      while (isRunning(1000))
      {
        if (!isRunning(1000))
        {
          break;
        }
      }
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
int checkButtPress(int *buttState, bool *buttPressStatus)
{
  for (int i = 1; i < BUTTONS_QUANTITY; i++)
  {
    if (digitalRead(buttInit[i]) == HIGH)
    {
      buttState[i] = 1;
      *buttPressStatus = true;
    }
  }
}

int checkResult(int num, int *buttState, bool *buttPressStatus)
{
  lcd.clear();
  if (!(*buttPressStatus)) {
    lcd.print("NO BUTTON PRESS(");
    return 0;
  }
  else if (buttState[num] == 1) {
    lcd.print("    CORRECT!");
    return 1;
  }
  else {
    lcd.print("   INCORRECT(");
    return 0;
  }
  return 0;
}

//clearing butt bufer
void clearButt(int *buttState, bool *buttPressStatus)
{
  buttState[0] = 1;
  *buttPressStatus = false;
  for (int i = 1; i < BUTTONS_QUANTITY; i++)
  {
    buttState[i] = 0;
  }
}

void userSetGameTime(int *buttState, bool *buttPressStatus)
{
  lcd.clear();
  lcd.print(" set game time!");
  lcd.setCursor(0, 1);
  lcd.print("1=10, 2=20, 3=30");
  while ( !(*buttPressStatus) )
  {
    checkButtPress(buttState, buttPressStatus);
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
        lcdTimer.set(2000);
        lcdTimer.dlay();
        countdown();
        timer.set(t * i); //set how long game will last
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
    lcdTimer.set(1000);
    lcdTimer.dlay();
  }
  lcd.clear();
  lcd.print("  game started ");
}

void printScore(int score)
{
  lcd.setCursor(0, 1);
  lcd.print("your score is:");
  lcd.print(score);
  Serial.print(score);
}

void printGameOverScore(int score)
{
  lcd.clear();
  lcd.print("   GAME OVER!");
  lcd.setCursor(0, 1);
  lcd.print(" final score:");
  lcd.print(score);
  Serial.print(score);
}

void printPressStartButton(int *buttState)
{
  lcd.clear();
  lcd.print("LED BUTTON GAME!");
  while (buttState[0] == 0)
  {
    if (digitalRead(buttInit[0]) == HIGH)
    {
      buttState[0] = 1;
    }
    lcd.setCursor(0, 1);
    lcd.print("press start butt");
    logoTimer.set(1000);
    logoTimer.specialDelayTimer(buttState);
    lcd.clear();
    lcd.print("LED BUTTON GAME!");
    logoTimer.set(1000);
    logoTimer.specialDelayTimer(buttState);
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
  int buttState[] = {0, 0, 0, 0};
  static int score = 0;
  static int gameState = 1;
  static bool buttPressStatus = false;
  
  switch (gameState)
  {

    case LOGO:
      printPressStartButton(buttState);
      clearButt(buttState, &buttPressStatus);
      userSetGameTime(buttState, &buttPressStatus);
      gameState = GAME_RUNNING;

    case GAME_RUNNING: // the game
      timer.timeLeft();
      clearButt(buttState, &buttPressStatus);
      timer.updt(); // updating and init timer value
      waitTimer.set(t);
      waitTimer.dlay();
      ledTimer.updt();
      if (timer.timeLeft() <= 999)
      {
        t = timer.timeLeft();
        gameState = GAME_SCORE;
      }
      else
      {
        ledOn(b); //turn on led
        ledTimer.set(t); //setting time for leds
        buttPressStatus = false;
        while (ledTimer.isRunning())
        {
          checkButtPress(buttState, &buttPressStatus);
          if (buttPressStatus)
          {
            break;
          }
        }
        score = score + (checkResult(b, buttState, &buttPressStatus));
        clearButt(buttState, &buttPressStatus); //clearing butt bufer
        ledOff(b); //turn off led
        printScore(score);
      }

      break;

    case GAME_SCORE:
      scoreTimer.set(5000);
      printGameOverScore(score);
      scoreTimer.dlay();
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
