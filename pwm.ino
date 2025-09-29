# define PIN_LED 7

unsigned int toggle;
unsigned int period;
unsigned int duty_int;

unsigned int duty;
double cycle;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  toggle = 1;
  period = 1000;
  duty = 50;
  cycle = 500000 / period;
}

void loop() {
  toggle = toggle_state(toggle);
  duty_int = set_duty(duty);
  digitalWrite(PIN_LED, toggle);
  delayMicroseconds(double(period/100)*(duty_int));
  toggle = toggle_state(toggle);
  digitalWrite(PIN_LED, toggle);
  delayMicroseconds(double(period/100)*(100-duty_int));
}

int toggle_state(int toggle){
  return !toggle;
}
int set_duty(double duty1){
  duty = duty1 +(100/cycle);
  if (duty >= 100 || duty <= 0) cycle = -cycle;
  return int(duty);
}
