//lookup table method sourced from:
//https://www.eevblog.com/forum/projects/rotary-encoder-jocks-a-question-about-the-lookup-table/
#include "Keyboard.h"

int EncA = 2; //GPIO #5-CLK on encoder (Output A)
int EncB = 3; //GPIO #4-DT on encoder (Output B)

volatile int counter = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(EncA,INPUT_PULLUP);
  pinMode(EncB,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(EncA), readEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(EncB), readEncoder, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = digitalRead(EncA);
}

void readEncoder() {
  static uint8_t oldAB = 3; //0000 0011 default starting value for encoders
  static int8_t encVal = 0;
  static const int8_t enc_states[]  = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Lookup table

  oldAB <<=2; //remembers previous value

  if (digitalRead(EncA)) oldAB |= 0x02; // Add current state of pin A
  if (digitalRead(EncB)) oldAB |= 0x01; // Add current state of pin B
  
  encVal += enc_states[( oldAB & 0x0f )];

  if(encVal > 3){
    Keyboard.press('4');
    Keyboard.release('4');
    counter++;
    encVal = 0;
  }
  else if (encVal < -3){
    Keyboard.press('3');
    Keyboard.release('3');
    counter--;
    encVal = 0;
  }

  Serial.print(counter);
}
