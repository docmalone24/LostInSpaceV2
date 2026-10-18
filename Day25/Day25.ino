#include <U8glib.h>
 
U8GLIB_SH1106_128X64  My_u8g_Panel(U8G_I2C_OPT_NONE); // I2C / TWI

// Rotary Encoder Inputs
#define CLK2 3
#define DT2 4
// Switch Input
#define SW2 2

//light sensor range is 0 - 671
int sensorPin = A1; //select the *analog zero* input pin for probing the photoresistor 
int senMax = 680; //maximum light value received by the sensor
int buzzer = 8;  //sound output pin
int redPin = 11;   //  PWM color output pins
int greenPin = 10;
int bluePin = 9;

int access = 0;
int length = 1;
int password = 0;

String enterPass = "";
String stars = "";
int pw2 = 0;
int pw1 = 0;
int multiplier = 10;

char menuOption = 'A';
char result = 'X';

int currentStateCLK;
int lastStateCLK;

void setup() {
  // Setup Serial Monitor
  Serial.begin(9600);
  // flip screen, if required
  // My_u8g_Panel.setRot180();
 
  // assign default color value
  My_u8g_Panel.setColorIndex(1); // pixel on

  // Set encoder pins as inputs
  pinMode(CLK2,INPUT);
  pinMode(DT2,INPUT);
  pinMode(SW2,INPUT_PULLUP);

    // Read the initial state of A (CLK)
  lastStateCLK = digitalRead(CLK2);

  // Call Interrupt Service Routine (ISR) updateEncoder() when any high/low change
  // is seen on A (CLK2) interrupt  (pin 2), or B (DT2) interrupt (pin 3)
  //hardware interrupts are only available on pins 2 and 3
  attachInterrupt(digitalPinToInterrupt(CLK2), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(SW2), record, CHANGE);

/*/ picture loop  
  My_u8g_Panel.firstPage();  
  do {
    passwordHeader();
  } while( My_u8g_Panel.nextPage() );*/
}

//  custom function to light the LED *******************************
void RGB_color(int red_value, int green_value, int blue_value){
  analogWrite(redPin, red_value);
  analogWrite(greenPin, green_value);
  analogWrite(bluePin, blue_value);
}

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

void menuOptA(){
  playSuccess();
  int sensorValue = 0;
  int buzzTone = 100;
  float senPercent = 0.0;
  Serial.println("Press the button to return to menu.");
  while(access == 2) {
    noInterrupts();
    // run photo sound operation until a key is pressed
    sensorValue = analogRead(sensorPin);
    senPercent = (float)sensorValue / (float)senMax;
    buzzTone = int(senPercent * 4000);
    Serial.print("Sensor: ");
    Serial.print(sensorValue);
    Serial.print("  Tone: ");
    Serial.print(buzzTone);
    Serial.print("  Should be: ");
    Serial.print(senPercent);
    Serial.print("  Access: ");
    Serial.println(access);
    tone(buzzer, buzzTone, 100);
    delay(10);
    noTone(buzzer);
    interrupts();
    delay(10);
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
    noInterrupts();
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
    delay(10);
    interrupts();
  }
  RGB_color(0,0,0);
  return(0);
} 

void showMenu(){
  if (menuOption == 'A'){
  My_u8g_Panel.setFont(u8g_font_unifont); //16 characters per line
  My_u8g_Panel.drawStr( 0, 10, "Enter an option:");
  My_u8g_Panel.drawBox(0,11,120,12);
  My_u8g_Panel.setColorIndex(0);
  My_u8g_Panel.drawStr( 0, 22, "A:Sound Pitch");
  My_u8g_Panel.setColorIndex(1);
  My_u8g_Panel.drawStr( 0, 34, "B:Light");
  My_u8g_Panel.drawStr( 0, 46, "C:New Password");
  My_u8g_Panel.drawStr( 0, 58, "E:Exit");
  }
  if (menuOption == 'B'){
  My_u8g_Panel.setFont(u8g_font_unifont); //16 characters per line
  My_u8g_Panel.drawStr( 0, 10, "Enter an option:");
  My_u8g_Panel.drawStr( 0, 22, "A:Sound Pitch");
  My_u8g_Panel.drawBox(0,23,120,12);
  My_u8g_Panel.setColorIndex(0);
  My_u8g_Panel.drawStr( 0, 34, "B:Light");
  My_u8g_Panel.setColorIndex(1);
  My_u8g_Panel.drawStr( 0, 46, "C:New Password");
  My_u8g_Panel.drawStr( 0, 58, "E:Exit");
  }
  if (menuOption == 'C'){
  My_u8g_Panel.setFont(u8g_font_unifont); //16 characters per line
  My_u8g_Panel.drawStr( 0, 10, "Enter an option:");
  My_u8g_Panel.drawStr( 0, 22, "A:Sound Pitch");
  My_u8g_Panel.drawStr( 0, 34, "B:Light");
  My_u8g_Panel.drawBox(0,35,120,12);
  My_u8g_Panel.setColorIndex(0);
  My_u8g_Panel.drawStr( 0, 46, "C:New Password");
  My_u8g_Panel.setColorIndex(1);
  My_u8g_Panel.drawStr( 0, 58, "E:Exit");
  }
  if (menuOption == 'E'){
  My_u8g_Panel.setFont(u8g_font_unifont); //16 characters per line
  My_u8g_Panel.drawStr( 0, 10, "Enter an option:");
  My_u8g_Panel.drawStr( 0, 22, "A:Sound Pitch");
  My_u8g_Panel.drawStr( 0, 34, "B:Light");
  My_u8g_Panel.drawStr( 0, 46, "C:New Password");
  My_u8g_Panel.drawBox(0,47,120,12);
  My_u8g_Panel.setColorIndex(0);
  My_u8g_Panel.drawStr( 0, 58, "E:Exit");
  My_u8g_Panel.setColorIndex(1);
  }
}

void loop() {
  if (access <= 0){
    My_u8g_Panel.firstPage();  
    do {
     showPassword();
    } while( My_u8g_Panel.nextPage() );
    delay(10);
  }
  if (access == 1){
    My_u8g_Panel.firstPage();  
    do {
     showMenu();
    } while( My_u8g_Panel.nextPage() );
    delay(10);
  }
}

//Draw the Enter Password header for menu access
void passwordHeader(){
  // graphic commands to redraw the complete screen should be placed here  
  My_u8g_Panel.setFont(u8g_font_unifont);
  My_u8g_Panel.drawStr( 0, 10, "Enter Password:");
}

void showPassword(){
  enterPass = String(pw2);
  //My_u8g_Panel.drawStr( 0, 30, pw1); //pw1 + pw2);
  My_u8g_Panel.setFont(u8g_font_unifont);
  My_u8g_Panel.drawStr( 0, 10, "Enter Password:");
  My_u8g_Panel.setPrintPos(0,22);
  My_u8g_Panel.print(stars + enterPass);
}

//  This is our ISR which has the job of responding to interrupt events
void updateEncoder(){
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK2);

  if (access == 0 || access == 3) {  //actions while validating password
    // If last and current state of CLK are different, then a pulse occurred;
    // React to only 0->1 state change to avoid double counting
    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
      // If the DT state is different than the CLK state then
      // the encoder is rotating CW so INCREASE counter by 1
      if (digitalRead(DT2) == currentStateCLK) {
        if(pw2 == 9){
          pw2 = 0;
        }
        else {
          pw2 ++;
        }
      } else {
        // Encoder is rotating CCW so DECREASE counter by 1
        if(pw2 == 0){
          pw2 = 9;
        }
        else {
          pw2 --;
        }
      }
    }
    // Remember last CLK state to use on next interrupt...
    lastStateCLK = currentStateCLK;
  }
  if (access == 1) {   //actions after password has been validated and system access has been granted
    //Serial.println("Here you need to code to scroll through the menu options");
    if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
      if (digitalRead(DT2) == currentStateCLK) {
        switch (menuOption) {
          case 'A':
            menuOption = 'B';
            break;
          case 'B':
            menuOption = 'C';
            break;
          case 'C':
            menuOption = 'E';
            break;
          default:
            menuOption = 'A';
            break;
        }
      } 
      else {
        switch (menuOption) {
          case 'B':
            menuOption = 'A';
            break;
          case 'C':
            menuOption = 'B';
            break;
          case 'E':
            menuOption = 'C';
            break;
          default:
            menuOption = 'E';
            break;
        }
      }
    }
    // Serial.print("Direction: ");
    // Serial.print(currentDir);
    // Serial.print(" | Counter= ");
    // Serial.println(counter);
    // Remember last CLK state to use on next interrupt...
    lastStateCLK = currentStateCLK;
  }
}

//function makes onboard LED flash when button is pushed
void record(){
  Serial.print("Record Access: ");
  Serial.println(access);
  delay(1000);
  // if (access > 0){
  // Serial.print("MenuOption: ");
  // Serial.println(menuOption);
  // }
  //Serial.println(digitalRead(SW2));

  if (access <= 0)  {   //actions while validating password
    if (digitalRead(SW2) == HIGH){
      //Serial.println("in loop");
      //pw2 = pw1;
      //multiplier = multiplier * 10;
      if (length == 4){ //entering of password complete, validate if correct
      //Serial.print("length 4 - pw1 = ");
      //Serial.println(pw1);
        pw1 = (pw1 * multiplier) + pw2;
        //Serial.print("length 4 - pw1 = ");
        //Serial.println(pw1);
        //length = 4;
        if (pw1 == password){
          Serial.println("Looks Good");
          playSuccess();
          My_u8g_Panel.firstPage();  
          do {
            My_u8g_Panel.setFont(u8g_font_unifont);
            //My_u8g_Panel.drawStr( 0, 10, "Enter Password:");
            My_u8g_Panel.setPrintPos(0,50);
            My_u8g_Panel.print("Entering menu...");
            //delay(20000);
          } while( My_u8g_Panel.nextPage() );
          access = 1;
          delay(200000);
          return(0);
          //display menu
        }
        else {
          /*if (result =='C'){
            Serial.println("Nope");
          }
          else {*/
            Serial.println("Fuck off imposter!");
            Serial.println("Enter the correct password to access the system:");
          //}
          My_u8g_Panel.firstPage();  
          do {
            My_u8g_Panel.setFont(u8g_font_unifont);
            //My_u8g_Panel.drawStr( 0, 10, "Enter Password:");
            My_u8g_Panel.setPrintPos(0,50);
            My_u8g_Panel.print("Nope...");
            //delay(20000);
          } while( My_u8g_Panel.nextPage() );
          delay(2000);
          pw1 = 0;
          pw2 = 0;
          length = 1;
          enterPass = "";
          stars = "";
          //result = 'X';
          //OurDisplay.clear();
          return(0);
        }
      }
      if (length < 4) { //while validating password
      pw1 = (pw1 * multiplier) + pw2;
      length ++;
      //multiplier = multiplier * 10;
      pw2 = 0;
      stars = String(stars + "*");
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
  }
  if (access == 1 && digitalRead(SW2) == HIGH){  //while in the main menu
    // Serial.print("MenuOption: ");
    // Serial.println(menuOption);
    //Serial.println("Add select a menu option");
    switch (menuOption) {
      case 'A':
        Serial.println("Option A");
        delay(1000);
        access = 2;
        menuOptA();
        result = 'A';
        return(0);
      case 'B':
        Serial.println("Option B");
        delay(1000);
        access = 2;
        menuOptB();
        result = 'B';
        return(0);
      case 'C':
        Serial.println("Option C");
        access = 2;
        //menuOptA();
        result = 'C';
        return(0);
      case 'E':
        access = 0;
        pw1 = 0;
        pw2 = 0;
        length = 1;
        enterPass = "";
        stars = "";
        menuOption = 'A';
        result = 'X';
        Serial.println("System exited");
        Serial.println("Enter password to access the system:");
        break;
      default:
        Serial.println("Select a menu option");
        break;
    }
  }
  if (access == 2 && digitalRead(SW2) == HIGH){  //while you are inside of a menu option
    //Serial.println("Add return to the menu state");
    access = 1;
    result = 'Y';
    delay(10);
    return(0);
    //showMenu();
    //Serial.println("exit menu A or B, Access was 2, now == 1");
  }
  /*
  if (access == 3 && digitalRead(SW2) == HIGH){  //switch operation while resetting password
    length ++;
    if (length < 5){
    pw2 = pw1;
    pw1 = pw1 * multiplier;
    }
  }  
*/}