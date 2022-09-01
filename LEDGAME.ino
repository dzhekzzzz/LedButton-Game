
#include <stdio.h> 
int a = 1;
int score = 0;
int noButtPress = 0;
int buttInit[4] = {4, 7, 10, 12} ;
int ledInit[4] = {3, 9, 5, 2} ;
int butt[5];

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
  unsigned long  i_timer;
  unsigned long  i_ledTimer;
  unsigned long  i_oldLedTimer;
  unsigned long  limitValue;
  unsigned long  old_i_timer;
  unsigned long  newTimeLed;

  void updateTimer()
  {
     i_timer = millis();
  }
  
    void setTimer(unsigned long timeToSet)
    {
      old_i_timer = millis();
      limitValue = timeToSet;
    }
  
      bool isTimerRunning()
      {
        i_timer = millis();
        
        if ( (i_timer - old_i_timer) <= limitValue)
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
int randomLed(int num)
{
   digitalWrite(ledInit[num], HIGH);   
}


//turn off all leds
void ledOff()
  {
      for (int l = 0; l < 4; l++)
      {
        digitalWrite(ledInit[l], LOW);
      } 
  }  

//выдаем результат в зависимости от нажатой кнопки 
int buttWait(int num)
{
      if (num == butt[num])
      {
        Serial.print("                                                                  CORRECT! \n");
        score += 1;
      }    
        else if ( ledTimer.isTimerRunning() == false)
        {
          Serial.print("                                                          NO BUTTON PRESS DETECTED \n");
        }

        return 0;
}

//clearing butt bufer
    void clearButt()
    {
        butt[0] = 1; 
      for (int i = 1; i < 5; i++)
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


void loop() 
{ 
  int b = randomGenLed();
  int t = randomGenTime();
  int timeleft = timer.limitValue - (timer.i_timer - timer.old_i_timer);
  switch (a) 
  {
  case 1: // the game start
        Serial.print("                                                        Please press Start Button!!!\n\n");
        startButtonWait(); //waiting for start button
        timer.setTimer(15000); //set how long game will last 
        timer.updateTimer(); // init timer
        a = 2;
        break;
        
  case 2: // the game
        timer.updateTimer(); // updating timer value
        randomLed(b); //turn on random led
        ledTimer.setTimer(t); //setting time for leds
        if ( (timeleft) <= 999)
         {
          t = timeleft;    
          a = 3;     
         }
        while ( ledTimer.isTimerRunning() == true )
        {
          if (digitalRead(buttInit[b]) == HIGH) 
           {
            butt[b] = b;
             break;
           }
        }
        buttWait(b);
        clearButt(); //clearing butt bufer
        ledOff(); //turn off all leds
        Serial.print("                                                              your score is ");
        Serial.print(score);
        Serial.print("\n \n");
        waitTimer.setTimer(t);
           while ( waitTimer.isTimerRunning() == true )
        {
          if ( waitTimer.isTimerRunning() == false )
          {
            break;
          }
           
        }
        break;     
        
  case 3: //game over
       Serial.print("                                                                 GAME OVER\n");
       Serial.print("                                                          your final score is ");
       Serial.print(score);
       Serial.print(" !!! \n\n\n");
       a = 1;
       score = 0;
       butt[0] = 0;
       break;
       
        
  default:
    break;
  }
}
