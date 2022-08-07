
#include <Arduino.h>
#include <HardwareSerial.h>

#define NUM_SAMPLES 10  // number of times we must measure a different value before incrementing counter
#define PHOTOTRANSISTOR_DIGITAL_PIN 2  // D2, interrupt capable
#define PHOTOTRANSISTOR_ANALOG_PIN A7

uint8_t lastDigitalVal = 0;  // 0 for off, 1 for on
uint16_t counter = 0;


void setup() {
  // for use with the digital looop
  pinMode(PHOTOTRANSISTOR_DIGITAL_PIN, INPUT);
  Serial.begin(9600);           //  setup serial
}

void loop() {
  // Choose one of the methods to change the program
  
  // Just print sampled analog values from A7
  // testAnalogLoop();

  // Just print sampled digital values from D2
  //testDigitalLoop();

  // Poll the pin for changes to the counter
  //directLoop();

  // Count using an interrupt
  interruptLoop();
}


void interruptLoop() {
  // install an interrupt to read the phototransistor pin. 
  attachInterrupt(digitalPinToInterrupt(PHOTOTRANSISTOR_DIGITAL_PIN), countInterrupt, RISING); 
}

void countInterrupt() {
  counter++;
  Serial.print("Counter: ");
  Serial.println(counter);
}

boolean isDifferent() {
  uint16_t currentAnalogVal;
  uint8_t currentDigitalVal;
  for (int i = 0 ; i < NUM_SAMPLES; i++) {
    currentAnalogVal = analogRead(PHOTOTRANSISTOR_ANALOG_PIN);  // read the input pin
    if (currentAnalogVal > 500) {
      currentDigitalVal = 1;
    } else {
      currentDigitalVal = 0;
    }
    if (lastDigitalVal == currentDigitalVal) {
      return false;
    }
  }
  Serial.print("Change to ");
  Serial.println(currentDigitalVal);
  lastDigitalVal = currentDigitalVal;
  return true;
}

void directLoop() {

  if (isDifferent()) {
    counter++;
    Serial.print(" Counter: ");
    Serial.println(counter);
  }
}

void testAnalogLoop() {
  uint16_t currentAnalogVal = analogRead(PHOTOTRANSISTOR_ANALOG_PIN);  
  Serial.println(currentAnalogVal);          // debug value
  delay(1000);
}

void testDigitalLoop() {
  uint16_t currentDigitalVal = digitalRead(PHOTOTRANSISTOR_DIGITAL_PIN);  
  Serial.println(currentDigitalVal);          // debug value
  delay(1000);
}
