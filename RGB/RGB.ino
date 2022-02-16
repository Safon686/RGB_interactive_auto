#include <EEPROM.h>
uint32_t timer, timer_button, timer_button2;
int interval = 5;                   //шаг таймера
int bright;                         //величина ШИМ в канал цвета
int dif;                            //лимит ШИП для яркости, можно изменять удерживая кнопку
bool flag;                          //переменная состояния кнопки
int RED = 9, GREEN = 10, BLUE = 11;
void setup() {
  Serial.begin(9600);
  pinMode(RED, OUTPUT);               //красный канал
  pinMode(GREEN, OUTPUT);              //Зеленый канал
  pinMode(BLUE, OUTPUT);              //Синий канал
  pinMode (5, INPUT);               //Кнопка
  pinMode (8, INPUT);               //пин считывания входящего сигнала с реле 5в,реле сигнал с 12в освещения салона
  pinMode (6, INPUT);               //пин под пустое реле
  pinMode (7, INPUT);               //пин под пустое реле
  analogWrite(9, 0);                //сток значение яркости канала
  analogWrite(10, 0);               //сток значение яркости канала
  analogWrite(11, 0);               //сток значение яркости канала
  pinMode (5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
  timer = millis();                 //таймер для многопточности :D
  //dif = 60;                         //даем дефолт, можно будет потом брать с памяти
  EEPROM.get(10, dif);              //Читаем из памяти
  bright = 0;                       //дефолт яркости, можно будет потом брать с памяти
  flag = false;                     //флаг смены функции кнопки
}


void fun_bright_high (int channel1, int channel2, int channel3) {     //плавно поднимаем яркость
  if ((bright >= 0) && (bright < 255)) {
    if (millis() - timer > interval) {
      analogWrite(channel1, bright);
      analogWrite(channel2, bright);
      analogWrite(channel3, bright);
      ;
      bright++;
      timer = millis();
    }
  }
  else {
    if ((millis() - timer > interval) && (bright >= 0) && (bright != 255)) { //контролируем, что записанная яркость не выше порога,
      analogWrite(channel1, bright);                                         //если выше, то уменьшаем
      analogWrite(channel2, bright);
      analogWrite(channel3, bright);
      bright--;
      timer = millis();
    }
  }
}
void fun_bright_low (int channel1, int channel2, int channel3) {          //плавно уменьшаем яркость синего и зеленого канала
  if ((bright > 0) && (bright < 9999)) {
    if (millis() - timer > interval) {
      analogWrite(channel1, dif);
      analogWrite(channel2, bright);
      analogWrite(channel3, bright);
      bright--;
      timer = millis();
    }
  }
  else {
    if ((millis() - timer > interval) && (bright < 260) && (bright != 0)) {   //контролируем, что записанная яркость не ниже порога,
      analogWrite(channel1, bright);                                          //если ниже, то поднимаем
      analogWrite(channel2, bright);
      analogWrite(channel3, bright);
      bright++;
      timer = millis();
    }
  }
}
void loop() {
  //EEPROM.get(10, dif);
  int Light_status = digitalRead(8);                    //читаем 5в с реле
  bool button = digitalRead(5);                         //читаем кнопку
  //Serial.println(flag);
  Serial.println(dif);
  //Serial.println(Light_status);
  //Serial.println(bright);
  /* if (button && !flag && millis() - timer_button > 500) { //корекция нажатия
     flag  = true; //поднимаем флаг
     timer_button = millis();
     Serial.println("Click!");
    } */
  if (button && !flag && millis() - timer_button > 100) {       //состояни удержание кнопки
    Serial.println("Hold+!");
    if ((millis() - timer_button) >= (interval * 2)) {
      timer_button = millis();
      dif = dif + 10;                                            //при удержании увеличиваем дефолтную яркость
      EEPROM.put(10, dif);                                       //пишем в память
      if (dif > 240) {
        flag = true;                                             //по достижении макс ШИМ инвертируем кнопку на функцию понижения ШИМ
      }
    }
  }
  if (button && flag && millis() - timer_button > 100) {        //состояни удержание кнопки
    Serial.println("Hold!-");
    if ((millis() - timer_button) >= (interval * 2)) {
      timer_button = millis();
      dif = dif - 10;                                           //при удержании уменьшаем дефолтную яркость
      EEPROM.put(10, dif);                                       //пишем в память
      if (dif < 10) {
        flag = false;                                           //по достижении макс ШИМ инвертируем кнопку на функцию повышения ШИМ
      }
    }
  }
  //Serial.println(Light_status);
  if (Light_status == 1) {
    fun_bright_high(RED, GREEN, BLUE);
  }
  if (Light_status == 0)
  {
    fun_bright_low(0, GREEN, BLUE);
    analogWrite(RED, dif);                                        //красная подсветка горит во время движения, интерактивно меняется яркость по функции диф
  }
}
