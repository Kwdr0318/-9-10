#define PIN_TRIG  12  // sonar sensor TRIGGER
#define PIN_ECHO  13  // sonar sensor ECHO
#define PIN_SERVO 10  // servo control pin

#define _DUTY_MIN 500   // servo full clockwise position (0°)
#define _DUTY_MAX 2500  // servo full counter-clockwise position (180°)
#define _SERVO_SPEED 0.3  // servo angular speed (degree/sec)

void setup() {
  pinMode(PIN_SERVO, OUTPUT);
}

void loop() {
  int start_angle = 0;
  int end_angle = 90;
  int diff = end_angle - start_angle;

  float delay_per_degree = 1000.0 / _SERVO_SPEED;  

  for (int angle = start_angle; angle <= end_angle; angle++) {
    int duty = map(angle, 0, 180, _DUTY_MIN, _DUTY_MAX);
    servoWrite(PIN_SERVO, duty);
    delay(delay_per_degree); 
  }

  while (1); 
}

void servoWrite(int pin, int duty_us) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(duty_us);
  digitalWrite(pin, LOW);
  delayMicroseconds(20000 - duty_us);
}

