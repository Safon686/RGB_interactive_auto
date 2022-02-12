uint32_t timer, timer_button, timer_button2;
int interval = 20;
int bright = 0;
int dif = 60;

void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode (5, INPUT);
  pinMode (8, INPUT);
  pinMode (6, INPUT);
  pinMode (7, INPUT);
  analogWrite(9, 0);
  analogWrite(10, 0);
  analogWrite(11, 0);
  pinMode (5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  timer = millis();
  timer_button = millis(); //таймер опроса кнопки
  timer_button2 = millis(); //таймер коррекции яркости по кнопке
  long previosmillis = 0;
}

void fun_bright_high (int channel1, int channel2, int channel3) {
  if (dif > bright >= 1) {
    if (millis() - timer > interval) {
      analogWrite(channel1, bright);
      analogWrite(channel2, bright);
      analogWrite(channel3, bright);
      //Serial.println(bright);
      bright++;

    }
  }
  if (bright >= dif) {
    analogWrite(channel1, bright);
    analogWrite(channel2, bright);
    analogWrite(channel3, bright);
  }
}
void fun_bright_low (int channel1, int channel2, int channel3) {
  if (999 > bright >= 1) {
    if (millis() - timer > interval) {
      analogWrite(channel2, bright);
      analogWrite(channel3, bright);
      //Serial.println(bright);
      bright--;
      timer = millis();
    }
  }
  if (bright == 0) {
    analogWrite(channel2, bright);
    analogWrite(channel3, bright);
  }
}
void loop() {
  int Light_status = digitalRead(8);
  static bool flag = false; //переменная состояния нажатия кнопки
  bool button = digitalRead(5); //читаем кнопку
  if (button && !flag && millis() - timer_button > 200) { //корекция нажатия
    flag  = true; //поднимаем флаг
    timer_button = millis();
    Serial.println("Click!");

  }
  if (button && !flag && millis() - timer_button > 800) { //состояни удержание кнопки
    timer_button = millis();
    Serial.println("Hold!");
    if (millis() - timer_button2 >= interval) {
      timer_button2 = millis();
      dif++;                                          //при удержании увеличиваем дефолтную яркость
    }
  }
  if (button && flag && millis() - timer_button > 800) {  //состояни удержание кнопки
    timer_button = millis();
    Serial.println("Hold!");
    if (millis() - timer_button2 >= interval) {
      timer_button2 = millis();
      dif--;                                          // //при удержании уменьшаем дефолтную яркость
    }
  }
  if (!button && flag) {
    flag  = false;
  }
  Serial.println(Light_status);
  if (Light_status == 1) {
    fun_bright_high(9, 10, 11);
  }
  if (bright != 0)
  {
    fun_bright_low(0, 10, 11);
  }
}
