uint32_t timer;
int interval = 20;
int bright = 0;
int dif = 60;


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode (8, INPUT);
  pinMode (6, INPUT);
  pinMode (7, INPUT);

  analogWrite(9, 0);
  analogWrite(10, 0);
  analogWrite(11, 0);
  timer = millis();
  long previosmillis = 0;

}

void fun_bright_high (int channel1, int channel2, int channel3) {
  if (dif > bright >= 1) {
    if (millis() - timer > interval) {
      analogWrite(channel1, bright);
      analogWrite(channel2, bright);
      analogWrite(channel3, bright);
      Serial.println(bright);
      bright++;
      timer = millis();
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
  Serial.println(Light_status);
  if (Light_status == 1) {
    fun_bright_high(9, 10, 11);
  }
  if (bright != 0)
  {
    fun_bright_low(0, 10, 11);
  }
}
