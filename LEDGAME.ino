
#include <stdio.h> 
int i = 0;
int gameState = 1;
int score = 0;
int noButtPress = 0;
int buttInit[4] = {4, 7, 10, 12} ;
int ledInit[4] = {3, 9, 5, 2} ;
int butt[4];
#define GAME_SET_N_START (1)
#define GAME_RUNNING (2)
#define GAME_END (3)

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
int randomLedOn(int num)
{
   digitalWrite(ledInit[num], HIGH);   
}


//turn off all leds
void ledOff(int num)
  {
    digitalWrite(ledInit[num], LOW);
  }  

//буфер с нажатыми кнопками
int WhichButtPressed()
{
  
  for (int c = 1; c < 4; c++)
  {
   if (digitalRead(buttInit[c]) == HIGH)
   {
     butt[c] = c;
     i = 1;
   }
  } 
  
}


int Logic(int num)
{
 if (butt[num] == num && i == 1)
 {
  score += 1;
  Serial.print("                                                                  CORRECT! \n");
  //correct
 }
  else if (butt[num] != num && i == 1)
  {
    Serial.print("                                                                 INCORRECT( \n");
    //incorrect
  }
   else if (ledTimer.isTimerRunning() == false && i == 0)
   {
    Serial.print("                                                          NO BUTTON PRESS DETECTED \n");
   }
}

//clearing butt bufer
    void clearButt()
    {
        butt[0] = 1; 
      for (int i = 1; i < 4; i++)
      {
        butt[i] = 0;
      }
    }

 //waiting for start button
void startButtonWait()
{
   while (butt[0] == 0) 
   {
     if(digitalRead(buttInit[0]) == HIGH)
     { 
      butt[0] = 1; 
     }
   }
}

void PrintScore()
{
   Serial.print("                                                              your score is ");
   Serial.print(score);
   Serial.print("\n \n");
}

void loop() 
{ 
  int b = randomGenLed();
  int t = randomGenTime();
  int timeleft = timer.limitValue - (timer.newTimeValue - timer.oldTimeValue);
  switch (gameState) 
  {
  case GAME_SET_N_START: // the game start
        Serial.print("                                                        Please press Start Button!!!\n\n");
        startButtonWait(); //waiting for start button
        timer.setTimer(15000); //set how long game will last 
        timer.updateTimer(); // init timer
        gameState = GAME_RUNNING;
        break;
        
  case GAME_RUNNING: // the game
        timer.updateTimer(); // updating timer value
        randomLedOn(b); //turn on random led
        ledTimer.setTimer(t); //setting time for leds
        if ( (timeleft) <= 999)
         {
          t = timeleft;    
          gameState = GAME_END;     
         }

         i = 0;
        while (ledTimer.isTimerRunning() == true )
        {
          
         WhichButtPressed();
        } 
        Logic(b);
        
        clearButt(); //clearing butt bufer
        ledOff(b); //turn off led
        PrintScore();
        waitTimer.setTimer(t);
        while ( waitTimer.isTimerRunning() == true )
        {
          if ( waitTimer.isTimerRunning() == false )
          {
            break;
          }
        }
        break;     

  case GAME_END: //game over
       Serial.print("                                                                 GAME OVER\n");
       Serial.print("                                                          your final score is ");
       Serial.print(score);
       Serial.print(" !!! \n\n\n");
       gameState = GAME_SET_N_START;
       score = 0;
       butt[0] = 0;
       break;
       
        
  default:
    break;
  }
}
