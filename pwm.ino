#define PIN_LED 7
int duty = 0;           
int step = 1;           
unsigned long period = 1000; 

void setup() {
  pinMode(PIN_LED, OUTPUT);
}

void loop() {
  duty += step;
  if (duty <= 0 || duty >= 100) {
    step = -step;  
    duty += step;
  }

  unsigned long onTime = period * duty / 100;
  unsigned long offTime = period - onTime;

  digitalWrite(PIN_LED, HIGH);
  delayMicroseconds(onTime);
  digitalWrite(PIN_LED, LOW);
  delayMicroseconds(offTime);
}


