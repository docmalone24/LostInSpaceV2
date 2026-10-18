const int buzzerPin = 6;

int redPin = 5;   //  PWM color output pins, 11 conflicts with tone()
int greenPin = 10;
int bluePin = 9;

//  custom function to light the LED *******************************
void RGB_color(int red_value, int green_value, int blue_value){
  analogWrite(redPin, red_value);
  analogWrite(greenPin, green_value);
  analogWrite(bluePin, blue_value);
}

void setup() {
  pinMode(buzzerPin, OUTPUT);
  tone(buzzerPin, 1000); // Play 1000 Hz tone continuously
}

void loop() {
  RGB_color(0, 125, 0); // LED to GREEN
  tone(buzzerPin, 1000);
  delay(200);
  noTone(buzzerPin);
  tone(buzzerPin, 2700);
  delay(1000);
  noTone(buzzerPin);
  //delay(1000);
  RGB_color(0, 0, 0);
  delay(1000);
}