//lookup table method sourced from:
//https://www.eevblog.com/forum/projects/rotary-encoder-jocks-a-question-about-the-lookup-table/
//full oop doesnt work as when init for each class is run the interrupt isp expects a static method to be called,si only partial oop to initialize each encoder.
#include "Keyboard.h"

struct EncoderInfo {
  int8_t oldAB; 
  int8_t encVal;
};

//setup for left encoder
int L_EncA = 0; //GPIO #5-CLK on encoder (Output A)
int L_EncB = 1; //GPIO #4-DT on encoder (Output B)
int8_t L_oldAB = 3; //0000 0011 default starting value for encoders
int8_t L_encVal = 0;
char L_left = '3';
char L_right = '4';

//setup for right encoder
int R_EncA = 2; //GPIO #5-CLK on encoder (Output A)
int R_EncB = 3; //GPIO #4-DT on encoder (Output B)
int8_t R_oldAB = 3; //0000 0011 default starting value for encoders
int8_t R_encVal = 0;
char R_left = '0';
char R_right = '-';

void setup() {
  // put your setup code here, to run once:
  //setup for right encoder
  pinMode(L_EncA,INPUT_PULLUP);
  pinMode(L_EncB,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(L_EncA), LeftEncoderChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(L_EncB), LeftEncoderChange, CHANGE);

  //setup for right encoder
  pinMode(R_EncA,INPUT_PULLUP);
  pinMode(R_EncB,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(R_EncA), RightEncoderChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(R_EncB), RightEncoderChange, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void LeftEncoderChange(){
  EncoderInfo L;
  L = readEncoder(L_EncA, L_EncB, L_oldAB, L_encVal,L_left,L_right);
  L_oldAB = L.oldAB;
  L_encVal = L.encVal;
}

void RightEncoderChange(){
  EncoderInfo R;
  R = readEncoder(R_EncA, R_EncB, R_oldAB, R_encVal,R_left,R_right);
  R_oldAB = R.oldAB;
  R_encVal = R.encVal;
}

struct EncoderInfo readEncoder(int EncA, int EncB, int8_t oldAB, int8_t encVal, char left[], char right[]) {
  struct EncoderInfo info;
  static const int8_t enc_states[]  = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Lookup table

  oldAB <<=2; //remembers previous value

  if (digitalRead(EncA)) oldAB |= 0x02; // Add current state of pin A
  if (digitalRead(EncB)) oldAB |= 0x01; // Add current state of pin B
  
  encVal += enc_states[( oldAB & 0x0f )];

  if(encVal > 3){
    Keyboard.press(right);
    Keyboard.release(right);
    encVal = 0;
  }
  else if (encVal < -3){
    Keyboard.press(left);
    Keyboard.release(left);
    encVal = 0;
  }

  info.oldAB = oldAB;
  info.encVal = encVal;
  return info;
}
