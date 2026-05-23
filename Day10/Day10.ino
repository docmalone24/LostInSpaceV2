// Explicitly include Arduino.h
#include "Arduino.h"
 
// Our photoresistor will give us a reading of the current light level on this analog pin
const byte PHOTORESISTOR_PIN = A0;  // Photoresistor analog pin
 
// RGB LED pins
const byte RED_PIN = 11;    // pin controlling the red leg of our RGB LED
const byte GREEN_PIN = 10;  // pin ccontrolling the green leg of our RGB LED
const byte BLUE_PIN = 9;    // pin ccontrolling the blue leg of our RGB LED

// Switch input pins
const byte ALWAYS_ON = 2;   // pin for the switch that would indicate the light should be always on
const byte ALWAYS_OFF = 3;  // pin for the switch that would indicate the light should be always off
 
const unsigned long BATTERY_CAPACITY = 50000;  // Maximum battery capacity
 
/*
 * Display a color on our RGB LED by providing an intensity for
 * our red, green and blue LEDs.
 */
void displayColor(
  byte red_intensity,    // red LED intensity (0-255)
  byte green_intensity,  // green LED intensity (0-255)
  byte blue_intensity    // blue LED intensity (0-255)
) {
  analogWrite(RED_PIN, red_intensity);      // write red LED intensity using PWM
  analogWrite(GREEN_PIN, green_intensity);  // write green LED intensity using PWM
  analogWrite(BLUE_PIN, blue_intensity);    // write blue LED intensity using PWM
}
 
void setup() {
  // Declare the RGB LED pins as outputs:
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Declare the switch pins as inputs
  pinMode(ALWAYS_ON, INPUT);
  pinMode(ALWAYS_OFF, INPUT);
 
  // Start serial monitor
  Serial.begin(9600);
}
 
void loop() {
  static unsigned long photo_in = 0;
   photo_in = analogRead(PHOTORESISTOR_PIN);  // Add current "charge amount" to our battery
  
  if (digitalRead(ALWAYS_ON) && !digitalRead(ALWAYS_OFF)){
    displayColor(128, 128, 128);  //On switch is on, off switch is off, turn light on
  } else if (digitalRead(ALWAYS_OFF) && !digitalRead(ALWAYS_ON)) {
    displayColor(0, 0, 0);  //Off switch is on, on switch is off, turn light off
  } else {
    if (photo_in <= 5) {
      displayColor(128, 128, 128);
    } else if (photo_in > 5 && photo_in <= 25) {
      displayColor(32, 32, 32);
    } else {
      displayColor(0, 0, 0);
    }
  }
 
  Serial.print("On Switch: ");  
  Serial.print(digitalRead(ALWAYS_ON));
  Serial.print(", Off Switch: ");
  Serial.print(digitalRead(ALWAYS_OFF));
  Serial.print(", Photo level: ");
  Serial.println(analogRead(PHOTORESISTOR_PIN));
 
  delay(100);  // Delay 1/10 of a second so displayed values don't scroll too fast
}