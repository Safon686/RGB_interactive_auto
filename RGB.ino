uint32_t timer;
int interval = 20;


void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  analogWrite(9, 0);
  analogWrite(10,0);
  analogWrite(11, 0);
  timer = millis(); 
  long previosmillis = 0;
  
  
}

void fun_bright_high (int channel) {
  int bright = 0;
  while (bright !=60) {
    if (millis()-timer > interval) {
      analogWrite(channel, bright);
    
    bright++;
    timer=millis();
    }
    
  
  }
}
  void fun_bright_low (int channel) {
  int bright = 60;
  while (bright !=0) {
     if (millis()-timer > interval) {
      analogWrite(channel, bright);
    
    bright--;
    timer=millis();
    }
    
                 
  }
  
}
void loop() {
   //fun_bright_high(9);
   fun_bright_high(10);
   fun_bright_low(9);
   fun_bright_high(11);
   fun_bright_low(10);
   fun_bright_high(9);
   fun_bright_low(11);
 
   
}
