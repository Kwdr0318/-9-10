// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25       // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100     // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300     // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // coefficent to convert duration to distance

#define _EMA_ALPHA 0.5    // EMA weight of new sample (range: 0 to 1)
                          // Setting EMA to 1 effectively disables EMA filter.

// global variables
unsigned long last_sampling_time;   // unit: msec
float dist_prev = _DIST_MAX;        // Distance last-measured
float dist_ema;                     // EMA distance

const int N = 30;
int sample[N] ; 
int index = 0;

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_TRIG,OUTPUT);
  pinMode(PIN_ECHO,INPUT);
  digitalWrite(PIN_TRIG, LOW);

  // initialize serial port
  Serial.begin(57600);
}

void sample_setup(int value){
  sample[index] = value;
  index = (index + 1) % N;
}
int getMedian() {
  int sorted[N];
  memcpy(sorted, sample, sizeof(sample));  // 원본 배열 복사

  // 버블 정렬 (간단한 정렬 알고리즘)
  for (int i = 0; i < N - 1; i++) {
    for (int j = 0; j < N - i - 1; j++) {
      if (sorted[j] > sorted[j + 1]) {
        int temp = sorted[j];
        sorted[j] = sorted[j + 1];
        sorted[j + 1] = temp;
      }
    }
  }
  // 중위수 반환
  if (N % 2 == 1) {
    return sorted[N / 2];  // 홀수일 때
  } else {
    return (sorted[N / 2 - 1] + sorted[N / 2]) / 2;  // 짝수일 때 평균
  }
}


void loop() {
  float dist_raw, dist_filtered, dist_median;
  
  if (millis() < last_sampling_time + INTERVAL)
    return;

  dist_raw = USS_measure(PIN_TRIG,PIN_ECHO);

  if ((dist_raw == 0.0) || (dist_raw > _DIST_MAX)) {
      dist_filtered = 300;
  } else if (dist_raw < _DIST_MIN) {
      dist_filtered = 100;
  } else {    // In desired Range
      dist_filtered = dist_raw;
      dist_prev = dist_raw;
  }

  dist_ema = _EMA_ALPHA * dist_filtered + (1-_EMA_ALPHA) * dist_ema;
  sample_setup(dist_raw);
  dist_median = getMedian();

  Serial.print("Min:");   Serial.print(_DIST_MIN);
  Serial.print(",raw:"); Serial.print(dist_raw);
  Serial.print(",ema:");  Serial.print(dist_ema);
  Serial.print(",median:");  Serial.print(dist_median);
  Serial.print(",Max:");  Serial.print(_DIST_MAX);
  Serial.println("");

  if ((dist_raw < _DIST_MIN) || (dist_raw > _DIST_MAX))
    digitalWrite(PIN_LED, 1);       // LED OFF
  else
    digitalWrite(PIN_LED, 0);       // LED ON

  last_sampling_time += INTERVAL;
}

float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm
}
