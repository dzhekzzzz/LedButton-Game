
#include <stdio.h> 
int a = 1;
int score = 0;
int num;
int buttInit[4] = {4, 7, 10, 12} ;
int ledInit[4] = {3, 9, 5, 2} ;
int butt[5];



void setup() {
  
    Serial.begin(9600); 
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

  void oldGameTimer()
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
     buttWait();
  }
  };


  
Timer timer; 
Timer ledTimer;
//
//bool isButtonPressed(int num);
//{
//  return (button[num] == 1);
//}
//
//int checkButton(int num){
//  if (digitalRead(buttInit[1]) == HIGH) {
//  butt[num] = true 
//  }
//}
//
//checkBUtton(num)
//while (!isButtonPressed(num)){
//  
//  checkBUtoon(num);
//  if timerisRun break abo return;
//  }

//буфер с нажатиями кнопок
void buttPress() {
     unsigned long  newTimeLed;
     timer.i_oldLedTimer = millis();
   
  while (butt[num] == 0)
  {
     newTimeLed = millis();
  if (digitalRead(buttInit[1]) == HIGH) {
    butt[1] = 1;
    butt[2] = 0;
    butt[3] = 0;
    break;
    } 
  if (digitalRead(buttInit[2]) == HIGH) { 
    butt[1] = 0;
    butt[2] = 2;
    butt[3] = 0;
    break;
      }  
  if (digitalRead(buttInit[3]) == HIGH) {
    butt[1] = 0;
    butt[2] = 0;
    butt[3] = 3;
    break;
        }
   else if ((newTimeLed - timer.i_oldLedTimer) >= ledTimer.limitValue)
    {
     butt[4] = 1;
     break;
    }
  }
  
  }

//включение рандомной кнопки
int random_led() {
   randomSeed(analogRead(0));
   num = rand() % 3 + 1; 
   
   if (num == 1) {
    digitalWrite(ledInit[1], HIGH);
    }
   else if (num == 2) {
    digitalWrite(ledInit[2], HIGH);
    } 
   else if (num == 3) {
     digitalWrite(ledInit[3], HIGH);
    }
   else {
    digitalWrite(ledInit[0], HIGH);
    }
    return num;
  }


//выключаю все кнопки 
void led_off()
  {
   digitalWrite(ledInit[1], LOW);
   digitalWrite(ledInit[2], LOW);
   digitalWrite(ledInit[3], LOW);
   digitalWrite(ledInit[0], LOW);
  }  

//ждем нажатие кнопки
void buttWait()
{
      buttPress();
      if (butt[num] == num)
      {
        Serial.print("                                                                  CORRECT! \n");
        score += 1;
      }    
        else if (butt[4] == 1) 
        {
          Serial.print("                                                          NO BUTTON PRESS DETECTED \n");
        }
          else if ( (butt[num] != num) )
           {
            Serial.print("                                                                INCORRECT((( \n");
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

void buttsWait()
{
  while (butt[0] == 0)
{
  checkButton();
}
}

//проверяю или нажата кнопка старт
void checkButton()
{
  if(digitalRead(buttInit[0]) == HIGH){
   butt[0] = 1;
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

for (int p = 0, p < 5, p++;)
{
  butt[i] = buttindex;
}

//
//int index = randomLed();
//int buttons[4];
//int leds[3];
//buttons[0];
//index = 0;
//buttons[index];
//buttons[index] "" leds[index];
//butons[index] = 0;
//index++;
