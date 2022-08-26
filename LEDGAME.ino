
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

    // here you can use cycle for to init all pins
    pinMode(ledInit[1], OUTPUT);
    pinMode(ledInit[2], OUTPUT);
    pinMode(ledInit[3], OUTPUT);
    pinMode(ledInit[0], OUTPUT);
    pinMode(buttInit[1], INPUT);
    pinMode(buttInit[2], INPUT);
    pinMode(buttInit[3], INPUT);
    pinMode(buttInit[0], INPUT);   
}

class Timer
{
  public:
  unsigned long  i_timer;
  unsigned long  i_ledTimer;
  unsigned long  i_oldLedTimer;
  unsigned long  limitValue;
  unsigned long old_i_timer;

  void oldGameTimer() // IMHO it's better to call this func setTimer 
  {
    old_i_timer = millis();
  }

  void updateTimer()
  {
     i_timer = millis();
  }
  
  void setTimer(unsigned long timeToSet)
  {
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

// here you have a function with a same name as in line 50 - what is it for?
bool isTimerRun()
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
void buttPress()
{
  noButtPress = 0;
  timer.i_oldLedTimer = millis();
   
  while (butt[num] == 0)
  {

  if (digitalRead(buttInit[num]) == HIGH) {
    butt[num] = num;
    break;
    } 
    
   else if ( isTimerRun() == false )
    {
       noButtPress = 1;
     break;
    }
  }
  
  }

//включение рандомной дампочки
int random_led()
{
   randomSeed(analogRead(0)); // this can be launched in the setup()
   num = rand() % 3 + 1; 
   digitalWrite(ledInit[num], HIGH);   
   return num; // here you return the num but you do not use it
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
void buttWait()
{
      buttPress();
      if (num == butt[num])
      {
        Serial.print("                                                                  CORRECT! \n");
        score += 1;
      }    
        else if (noButtPress == 1)
        {
          Serial.print("                                                          NO BUTTON PRESS DETECTED \n");
        }
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
  while (butt[0] == 0) // here you use the global variable instead of the local variable throwing between the funcs
{
  checkButton();
}
}

//проверяю или нажата кнопка старт
void checkButton()
{
  if(digitalRead(buttInit[0]) == HIGH){ 
   butt[0] = 1; // here you use the global variable instead of the local variable throwing between the funcs
  }
}

void loop() { 

  switch (a) 
  {
  case 1: // the game start
        Serial.print("                                                        Please press Start Button!!!\n\n");
        buttsWait();
        timer.setTimer(15000); //сколько будет идти игра
        timer.oldGameTimer();
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
        random_led();
        ledTimer.setTimer(500); //ставим сколько дает времени на нажатие
        timer.Led_timer(); //включаю таймер 
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
