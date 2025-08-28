/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
// char hexaKeys[ROWS][COLS] = {
//   {'0','1','2','3'},
//   {'4','5','6','7'},
//   {'8','9','A','B'},
//   {'C','D','E','F'}
// };
char hexaKeys[ROWS][COLS] = {
  {'3','7','B','F'},
  {'2','6','A','E'},
  {'1','5','9','D'},
  {'0','4','8','C'}
};
//byte rowPins[ROWS] = {3, 2, 1, 0}; //connect to the row pinouts of the keypad
//byte colPins[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad
const byte ROW_PINS[ROWS] = { 5, 4, 3, 2 };
const byte COL_PINS[COLS] = { 6, 7, 8, 9 };

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), ROW_PINS, COL_PINS, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Serial.println(customKey);
  }
}
