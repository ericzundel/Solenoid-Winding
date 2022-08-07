
#include <Arduino.h>
#include <HardwareSerial.h>
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define PHOTOTRANSISTOR_DIGITAL_PIN 2  // D2, interrupt capable
#define RESET_BUTTON_DIGITAL_PIN 3     // D3, interrupt capable

uint16_t lastCounterValue = 0;
uint16_t counter = 0;

// Onetime setup
void setup() {
  pinMode(PHOTOTRANSISTOR_DIGITAL_PIN, INPUT);
  pinMode(RESET_BUTTON_DIGITAL_PIN, INPUT_PULLUP);

  // install an interrupt to read the phototransistor pin. 
  attachInterrupt(digitalPinToInterrupt(PHOTOTRANSISTOR_DIGITAL_PIN), count_interrupt, RISING);
  // install an interrupt to reset the counter
  attachInterrupt(digitalPinToInterrupt(RESET_BUTTON_DIGITAL_PIN), reset_interrupt, FALLING);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Count");
  
  Serial.begin(9600);           //  setup serial
}

// Main Loop
void loop() {
  // Poll the counter looking for changes every once in a while

  // Minimize reads from 'counter' by storing in a tmp variable as it is changed in the intterupt
  uint16_t tmpCounter = counter; 
  if (lastCounterValue != tmpCounter) {
    Serial.println(tmpCounter);
    lastCounterValue = tmpCounter;
  } else {
    Serial.print(".");
  }
  delay(500);

  // Refresh the LCD Display
  print_lcd_count(tmpCounter); 
}

// When the Phototransistor sees light, current flows through and pulls down the 
// Voltage at R2
void  count_interrupt() {
  // TODO: Debounce?
  counter++;
}

void reset_interrupt() {
  // TODO: Debounce?
  counter = 0;
}

void print_lcd_count(uint16_t val) {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(7, 0);
  // print the number of seconds since reset:
  lcd.print(val);
}
