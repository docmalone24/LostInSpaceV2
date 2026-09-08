// Explicitly include Arduino.h
#include "Arduino.h"

//Goal: recreate day 15 project using rotary encoder in the place of the keypad
#include <TM1637Display.h>

//include EncoderButton.h
#include <EncoderButton.h>

// Define the display connection pins:
#define CLK 7
#define DIO 6

// Rotary Encoder Inputs
#define CLK2 3
#define DT2 4
// Switch Input
#define SW2 2

EncoderButton eb1(CLK2, DT2, SW2);

//light sensor range is 0 - 671
int sensorPin = A0; //select the *analog zero* input pin for probing the photoresistor 
int senMax = 680; //maximum light value received by the sensor
int buzzer = 8;  //sound output pin
int redPin = 11;   //  PWM color output pins
int greenPin = 10;
int bluePin = 9;

int access = 0;

// Create display object of type TM1637Display:
TM1637Display OurDisplay = TM1637Display(CLK, DIO);
 
// Create array that turns all segments on:
const uint8_t data[] = {0xff, 0xff, 0xff, 0xff};
// Create array that turns all segments off:
const uint8_t blank[] = {0x00, 0x00, 0x00, 0x00};
// Characters to display while in the menu
const uint8_t aChar[] = {SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G};
const uint8_t bChar[] = {SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G};
const uint8_t cChar[] = {SEG_A | SEG_D | SEG_E | SEG_F};
const uint8_t eChar[] = {SEG_A | SEG_D | SEG_E | SEG_F | SEG_G};

//  menu option currently being displayed
char menuOption = 'X';
char result = 'X';

int pw1 = 0;
int pw2 = 0;
int length = 1;
int password = 2469;
int multiplier = 10;
 
//  custom functions to give audio feedback *******************************
 
void playSuccess() {
  RGB_color(0, 125, 0); // LED to GREEN
  tone(buzzer, 1000, 200);
  delay(200);
  tone(buzzer, 2700, 1000);
  delay(1000);
  noTone(buzzer);
  delay(1000);
  RGB_color(0, 0, 0);
}
 
void playError() {
  RGB_color(255, 0, 0); // LED to RED
  tone(buzzer, 147, 1000);
  delay(1000);
  noTone(buzzer);
  delay(1000);
  RGB_color(0, 0, 0);
}
 
void playInput() {
  tone(buzzer, 880, 200);
  delay(50);
  noTone(buzzer);
}
 
//  custom function to light the LED *******************************
void RGB_color(int red_value, int green_value, int blue_value){
  analogWrite(redPin, red_value);
  analogWrite(greenPin, green_value);
  analogWrite(bluePin, blue_value);
}

void setup() {
  pinMode(redPin, OUTPUT);  // designate pins for PWM LED output
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(13, OUTPUT);
 
  // Clear the display:
  OurDisplay.clear();
  delay(1000);
  OurDisplay.setBrightness(7);

  //Handler functions for new encoder library
  eb1.setEncoderHandler(updateEncoder);
  eb1.setReleasedHandler (record);
  eb1.useQuadPrecision(false);

  RGB_color(125, 125, 125);  //set LED to white on startup...
  delay(1000);
  RGB_color(0, 0, 0);  //... and off again
 
  Serial.begin(9600); // Begin monitoring via the serial monitor
  delay(1000);

  Serial.println("Enter password to access the system:");
}

void showMenu(){
  Serial.println("Enter an option:");
  Serial.println("A: Light In Controls Sound Pitch");
  Serial.println("B: Light In Controls Light Out");
  Serial.println("C: Change Password");
  Serial.println("E: Exit");
}

void passwordChange() {
  access = 0;
  pw1 = 0;
  pw2 = 0;
  length = 1;
  while (result == 'C'){
    OurDisplay.showNumberDec(pw1,true,length,0);
    delay(10);
    eb1.update();

    if (access == 1){
      //password entered correctly, start new loop
      access = 3;
      Serial.println("Enter new password:");
      OurDisplay.clear();
      result = 'N';
      pw1 = 0;
      pw2 = 0;
      length = 1;
    }
  }
  while (result == 'N'){
    //here enter code to write a new password
    //might need more interupt code 
    OurDisplay.showNumberDec(pw1,true,length,0);
    delay(10);
    eb1.update();
    if (length > 4){
      password = pw1;
      Serial.print("Password reset: ");
      Serial.println(password);
      result = 'C';
    }
  }
  OurDisplay.clear();
  showMenu();
  access = 1;
  result = 'X';
  return(0);
}

void menuOptA(){
  int sensorValue = 0;
  int buzzTone = 100;
  float senPercent = 0.0;
  Serial.println("Press the button to return to menu.");
  while(access == 2) {
    // run photo sound operation until a key is pressed
    sensorValue = analogRead(sensorPin);
    senPercent = (float)sensorValue / (float)senMax;
    buzzTone = int(senPercent * 4000);
    // Serial.print("Sensor: ");
    // Serial.print(sensorValue);
    // Serial.print("  Tone: ");
    // Serial.print(buzzTone);
    // Serial.print("  Should be: ");
    // Serial.println(senPercent);
    tone(buzzer, buzzTone, 100);
    delay(10);
    noTone(buzzer);
    delay(10);
    eb1.update();
  }
  return(0);
}

void menuOptB(){
  int sensorValue = 0;
  int lightLevel = 0;
  float senPercent = 0.0;
  int redLevel;
  int greenLevel;
  int blueLevel;
  Serial.println("Press button to return to menu.");
  while(access == 2) {
    // run photo sound operation until a key is pressed
    sensorValue = analogRead(sensorPin);
    senPercent = (float)sensorValue / (float)senMax;
    lightLevel = int(senPercent * 255);

    //Test variables
    // Serial.print("Sensor: ");
    // Serial.print(sensorValue);
    // Serial.print("  Tone: ");
    // Serial.print(lightLevel);
    // Serial.print("  Should be: ");
    // Serial.println(senPercent);

    //Option 1: increasing red for the first third, green for the middle, blue for the last 
    // if(lightLevel < 33){
    //   redLevel = ((float)lightLevel / 32.0) * 100;
    //   RGB_color(redLevel, 0, 0);
    // }
    // if(lightLevel >= 33 && lightLevel < 66){
    //   greenLevel = (((float)lightLevel - 32.0) / 33.0) * 100;
    //   RGB_color(0, greenLevel, 0);
    // }
    // if(lightLevel >= 66){
    //   blueLevel = (((float)lightLevel - 65.0) / 35.0) * 100;
    //   RGB_color(0, 0, blueLevel);
    // }
    
    //Option 2: all white light
    RGB_color(lightLevel, lightLevel, lightLevel);

    eb1.update();
  }
  RGB_color(0,0,0);
  return(0);
} 

void loop() {
 
 if (access <= 0){
  OurDisplay.showNumberDec(pw1,true,length,0);
  delay(10);
  eb1.update();
 }
  //debuging readout 
    // Serial.print("pw1: ");
    // Serial.print(pw1);
    // Serial.print("  length: ");
    // Serial.print(length);
    // Serial.print("  password: ");
    // Serial.print(password);
    // Serial.print("  multiplier: ");
    // Serial.print(multiplier);
    // Serial.print("  access: ");
    // Serial.println(access);

  while (access == 1) {           // enter menu mode
    //Serial.println("In the menu loop"); 
    delay(1);
    eb1.update();
   
    if (result == 'A'){
      //Serial.println("This is option A.");
      menuOptA();
    }
    if (result == 'B'){
      //Serial.println("This is option B");
      menuOptB();
    }
    if (result == 'C'){
      Serial.println("Enter current password:");
      passwordChange();
    }
  }
}
 
//  This is our ISR which has the job of responding to interrupt events
void updateEncoder(){
  
  if (access == 0 || access == 3) {  //actions while validating password
      if (eb1.increment() == 1) { //encoder has been turned clockwise
        if((pw1 - (pw2 * multiplier)) == 9){
          pw1 = pw2 * multiplier;
        }
        else {
          pw1 ++;
        }
      } else {
        // Encoder is rotating CCW so DECREASE counter by 1
        if(pw1 == (pw2 * multiplier)){
          pw1 = pw1 + 9;
        }
        else {
          pw1 --;
        }
      }
  }
  if (access == 1) {   //actions after password has been validated and system access has been granted
    //Serial.println("Here you need to code to scroll through the menu options");
      if (eb1.increment() == 1) {
        switch (menuOption) {
          case 'A':
            menuOption = 'B';
            OurDisplay.setSegments(bChar,1,3);
            break;
          case 'B':
            menuOption = 'C';
            OurDisplay.setSegments(cChar,1,3);
            break;
          case 'C':
            menuOption = 'E';
            OurDisplay.setSegments(eChar,1,3);
            break;
          default:
            menuOption = 'A';
            OurDisplay.setSegments(aChar,1,3);
            break;
        }
      } 
      else {
        switch (menuOption) {
          case 'B':
            menuOption = 'A';
            OurDisplay.setSegments(aChar,1,3);
            break;
          case 'C':
            menuOption = 'B';
            OurDisplay.setSegments(bChar,1,3);
            break;
          case 'E':
            menuOption = 'C';
            OurDisplay.setSegments(cChar,1,3);
            break;
          default:
            menuOption = 'E';
            OurDisplay.setSegments(eChar,1,3);
            break;
        }
      }
  }
}

//function makes onboard LED flash when button is pushed
void record(){
  if (access <= 0)  {   //actions while validating password
      pw2 = pw1;
      if (length == 4){ //entering of password complete, validate if correct
        if (pw1 == password){
          Serial.println("Looks Good");
          access = 1;
          OurDisplay.clear();
          if (result == 'X'){
            showMenu();
          }
          return(0);
        }
        else {
          if (result =='C'){
            Serial.println("Nope");
          }
          else {
            Serial.println("Fuck off imposter!");
            Serial.println("Enter the correct password to access the system:");
          }
          pw1 = 0;
          pw2 = 0;
          length = 1;
          result = 'X';
          OurDisplay.clear();
          return(0);
        }
      }
      if (length < 4) { //while validating password
      length ++;
      pw1 = pw1 * multiplier;
      }
    //debuging readout 
      // Serial.print("pw1: ");
      // Serial.print(pw1);
      // Serial.print("  pw2: ");
      // Serial.print(pw2);
      // Serial.print("  length: ");
      // Serial.print(length);
      // Serial.print("  password: ");
      // Serial.print(password);
      // Serial.print("  multiplier: ");
      // Serial.println(multiplier);
  }
  
  if (access == 1){  //while in the main menu
    switch (menuOption) {
      case 'A':
        Serial.println("Option A");
        access = 2;
        OurDisplay.clear();
        //menuOptA();
        result = 'A';
        return(0);
      case 'B':
        Serial.println("Option B");
        access = 2;
        OurDisplay.clear();
        //menuOptA();
        result = 'B';
        return(0);
      case 'C':
        Serial.println("Option C");
        access = 2;
        OurDisplay.clear();
        //menuOptA();
        result = 'C';
        return(0);
      case 'E':
        access = 0;
        pw1 = 0;
        pw2 = 0;
        length = 1;
        result = 'X';
        OurDisplay.clear();
        Serial.println("System exited");
        Serial.println("Enter password to access the system:");
        break;
      default:
        Serial.println("Select a menu option");
        break;
    }
  }
  if (access == 2){  //while you are inside of a menu option
    access = 1;
    result = 'Y';
    delay(10);
    showMenu();
  }
  if (access == 3){  //switch operation while resetting password
    length ++;
    if (length < 5){
    pw2 = pw1;
    pw1 = pw1 * multiplier;
    }
  }  
}