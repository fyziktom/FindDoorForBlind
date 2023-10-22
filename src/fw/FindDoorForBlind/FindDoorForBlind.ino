
#include <M5StickC.h>

#define SPEAKER_PIN 26
#define SPEAKER_CHANNEL 0
#define SPEAKER_VOLUME 7
#define TRIGGER_PIN 33
#define ECHO_PIN 32

#define MINLENGTH 50
#define MAXLENGTH 620
#define LIMITLENGTH 1500

#define SAMPLES_COUNT 8

bool captured = false;

int frequency = 5000;
int resolution = 8;
uint16_t dutyCycle = 125;

int pauseLength = 0;

float dist = 0;
float samples[SAMPLES_COUNT];
int sampleIndex = 0;

const int detectionWindowSize = 3;
bool detectionWindow[detectionWindowSize];
int detectionIndex = 0;

int i = 0;

void setup() {
  
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(30, 70);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);

  //setup speaker
  ledcSetup(SPEAKER_CHANNEL, frequency, resolution);
  ledcAttachPin(SPEAKER_PIN, SPEAKER_CHANNEL);
  ledcWriteTone(SPEAKER_CHANNEL, 0);
  digitalWrite(SPEAKER_PIN, 0); //this will mute spekaer
    
  // setup UZV PINS
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Init samples array to 0
  for (int i = 0; i < SAMPLES_COUNT; i++) {
      samples[i] = 0.0;
  }

  for (int i = 0; i < detectionWindowSize; i++) {
      detectionWindow[i] = false;
  }
}

void playTone(int value){
  if (value < MINLENGTH) {  
     ledcWriteTone(0, 261);
     ledcWrite(SPEAKER_CHANNEL, 0x400 / 2);
  }
  else if (value > 50 && value <= 200){
     ledcWriteTone(0, 294);
     ledcWrite(SPEAKER_CHANNEL, 0x400 / 2);
  }
  else if (value > 200 && value <= 300){
     ledcWriteTone(0, 329);  
     ledcWrite(SPEAKER_CHANNEL, 0x400 / 2);
  }
  else if (value > 300 && value <= 400){
     ledcWriteTone(0, 349); 
     ledcWrite(SPEAKER_CHANNEL, 0x400 / 2);  
  }
  else if (value > 400 && value <= 500){
     ledcWriteTone(0, 392); 
     ledcWrite(SPEAKER_CHANNEL, 0x400 / 2);   
  }
  else if (value > 500 && value <= 600){
     ledcWriteTone(0, 440); 
     ledcWrite(SPEAKER_CHANNEL, 0x400 / 2);   
  }
  else if (value > 600 && value <= MAXLENGTH){
     ledcWriteTone(0, 493);  
     ledcWrite(SPEAKER_CHANNEL, 0x400 / 2);    
  }
  else if (value > MAXLENGTH){
     ledcWriteTone(0, 0);      
  }
}


float measureDist() {

  // Clears the trigPin
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  float duration = pulseIn(ECHO_PIN, HIGH);

  // Calculating the distance
  float distance= duration*0.034/2;
  distance *= 10;

  return (float)distance;
}

float computeMovingAverage(float newDist) {
    if (newDist > MINLENGTH && newDist < LIMITLENGTH) {
        
        samples[sampleIndex] = newDist;
        sampleIndex = (sampleIndex + 1) % SAMPLES_COUNT; // update index

        // Calc actual average value
        double total = 0.0;
        for (int i = 0; i < SAMPLES_COUNT; i++) {
            total += samples[i];
        }
        return total / SAMPLES_COUNT;
    }
    return 0.0;
}

bool checkDetection(double value) {
    // Update window
    if (value > MINLENGTH && value < MAXLENGTH) {
        detectionWindow[detectionIndex] = true;
    } else {
        detectionWindow[detectionIndex] = false;
    }
    detectionIndex = (detectionIndex + 1) % detectionWindowSize;

    // Check last values
    return detectionWindow[0] && detectionWindow[1] && detectionWindow[2];
}

void loop() {
  
    dist = measureDist();
    double averageValue = computeMovingAverage(dist);

    if (averageValue > 0.0) {
        pauseLength = (int)averageValue;

        M5.Lcd.setCursor(0, 70);
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.print(pauseLength);

        if (checkDetection(averageValue)) {
            playTone(pauseLength);
        }
        else {
          ledcWriteTone(0, 0);
        }
    }

    delay(1);
}
