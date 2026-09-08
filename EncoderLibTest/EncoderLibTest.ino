#include <EncoderButton.h>

const byte ENCODER_CLK = 3;
const byte ENCODER_DT = 4;
const byte SWITCH_PIN = 2;

EncoderButton eb1(ENCODER_CLK, ENCODER_DT, SWITCH_PIN);

// Create one or more callback functions 
void onEb1Encoder(EncoderButton& eb) {
  Serial.print("eb1 incremented by: ");
  Serial.println(eb.increment());
  Serial.print("eb1 position is: ");
  Serial.println(eb.position());
}

void onEb1Button(EncoderButton& eb) {
  Serial.println("eb1 button has been released");
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

eb1.setEncoderHandler(onEb1Encoder);
eb1.setReleasedHandler (onEb1Button);
eb1.useQuadPrecision(false);

delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
eb1.update();
delay(100);
}