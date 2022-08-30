
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
  unsigned long old_i_timer;

  void updateTimer()
  {
     i_timer = millis();
  }
  
    void setTimer(unsigned long timeToSet)
    {
      old_i_timer = millis();
      limitValue = timeToSet;
    }
  
      bool isTimerRun()
      {
        if ( (i_timer - old_i_timer) <= limitValue)
          return true;
        else 
          return false;
      }

};

Timer timer; 
Timer ledTimer;


bool isLedTimerRun()
{
  unsigned long  newTimeLed;
  newTimeLed = millis();
  if ( (newTimeLed - timer.i_oldLedTimer) >= ledTimer.limitValue )
  {
    return false;
  }
    else
    {
      return true;
    }
  
}

int buttPress(int num)
{
  noButtPress = 0;
  timer.i_oldLedTimer = millis();
   
  while (butt[num] == 0)
 {

   if (digitalRead(buttInit[num]) == HIGH) 
     {
      butt[num] = num;
      break;
     } 
    
      else if ( isLedTimerRun() == false )
      {
       noButtPress = 1;
       break;
      }
 }
    return 0;
}

//generating random number for led
int RandomGenLed()
{
  int num = rand() % 3 + 1; 
   return num; 
}

//generating random time for led
int RandomGenTime()
{
  int randomtime = (rand() % 9 + 3) * 100; 
  return randomtime;
}

 //turn on random led
int random_led(int num)
{
   digitalWrite(ledInit[num], HIGH);   
}


//turn off all leds
void led_off()
  {
      for (int l = 0; l < 4; l++)
      {
        digitalWrite(ledInit[l], LOW);
      } 
  }  

//выдаем результат в зависимости от нажатой кнопки 
int buttWait(int num)
{
      buttPress(num);
      if (num == butt[num])
      {
        Serial.print("                                                                  CORRECT! \n");
        score += 1;
      }    
        else if (noButtPress == 1)
        {
          Serial.print("                                                          NO BUTTON PRESS DETECTED \n");
        }

        return 0;
}

//clearing butt bufer
    void clearbutt()
    {
        butt[0] = 1; 
      for (int i = 1; i < 5; i++)
      {
        butt[i] = 0;
      }
    }

 //waiting for start button
void StartButtonWait()
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
  int b = RandomGenLed();
  int t = RandomGenTime();
  
  switch (a) 
  {
  case 1: // the game start
        Serial.print("                                                        Please press Start Button!!!\n\n");
        StartButtonWait(); //waiting for start button
        timer.setTimer(15000); //set how long game will last 
        timer.updateTimer(); // init timer
        a = 2;
        break;
        
  case 2: // the game
        if (timer.isTimerRun() == false) //checking is timer run
        {
        a = 3;
        }
        timer.updateTimer(); // updating timer valuerun
        random_led(b); //turn on random led
        ledTimer.setTimer(t); //setting time for leds
        buttWait(b);
        clearbutt(); //clearing butt bufer
        led_off(); //turn off all leds
        Serial.print("                                                              your score is ");
        Serial.print(score);
        Serial.print("\n \n");
        delay(500);
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
