
#include <stdio.h> 
int a = 1;
int score = 0;
int noButtPress = 0;
int num;
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

  //таймер лампочки 3сек
  void Led_timer() 
  {
     buttWait(); // it is not right to do so - LedTImer mus be a led timer - it do not have to deal with buttons. You better run buttonWait in the loop()
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

//ожидаем нажатие кнопки 
int buttPress(int num)
{
  noButtPress = 0;
  timer.i_oldLedTimer = millis();
   
  while (butt[num] == 0)
  {

  if (digitalRead(buttInit[num]) == HIGH) {
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


int randomgen()
{
  int num = rand() % 3 + 1; 
   return num; 
}

//включение рандомной лампочки
int random_led(int num)
{
   digitalWrite(ledInit[num], HIGH);   
}



//выключаю все кнопки 
void led_off()
  {
    // here you also can use a cycle
   digitalWrite(ledInit[1], LOW);
   digitalWrite(ledInit[2], LOW);
   digitalWrite(ledInit[3], LOW);
   digitalWrite(ledInit[0], LOW);
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

//очистка буфера
    void clearbutt()
    {
        butt[0] = 1; 
      for (int i = 1; i < 5; i++)
      {
        butt[i] = 0;
      }
    }

//ждем кнопку старт
void buttsWait()
{
  while (butt[0] == 0) 
{
  if(digitalRead(buttInit[0]) == HIGH){ 
   butt[0] = 1; 
}
}
}

void loop() { 
  int b = randomgen();
  switch (a) 
  {
  case 1: // the game start
        Serial.print("                                                        Please press Start Button!!!\n\n");
        buttsWait();
        timer.setTimer(15000); //сколько будет идти игра
        timer.updateTimer(); // включаем таймер
        a = 2;
        break;
  case 2: // the game
        if (!timer.isTimerRun()) 
        {
        a = 3;
        }
        timer.updateTimer(); // обновляем таймер
        timer.isTimerRun();
        random_led(b);
        ledTimer.setTimer(500); //ставим сколько дает времени на нажатие
        buttWait(b);
        clearbutt(); //очищаю значение кнопок  
        led_off(); //выключаем все лампочки
        Serial.print("                                                              your score is ");
        Serial.print(score);
        Serial.print("\n \n");
        delay(500);

        break;     
  case 3:
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