//lookup table method sourced from:
//https://www.eevblog.com/forum/projects/rotary-encoder-jocks-a-question-about-the-lookup-table/
//full oop doesnt work as when init for each class is run, the interrupt expects a static method to be called,hence only partial oop for the encoder class.
//(can be fixed with static classes but then it would not be possible to instantiate multiple encoder classes,a limitation with the arduino language in sacrifice for readability)

#include "Keyboard.h"

class Encoder{
  public:
    int A;
    int B;
    int8_t oldAB;
    int8_t encVal;
    char leftKey;
    char rightKey;

    Encoder(int A,int B,char leftKey,char rightKey){
      this->A = A; //GPIO #5-CLK on encoder (Output A)
      this->B = B; //GPIO #4-DT on encoder (Output B)
      oldAB = 3; //0000 0011 default starting value for encoders
      encVal = 0;
      this->leftKey = leftKey;
      this->rightKey = rightKey;
    }

    void init(){ // init missing interrupts as explained above
      pinMode(A,INPUT_PULLUP);
      pinMode(B,INPUT_PULLUP);
    }
};

Encoder leftEnc(0,1,'3','4');
Encoder rightEnc(2,3,'0','-');

void setup() {
  // put your setup code here, to run once:
  //setup for right encoder
  leftEnc.init();
  attachInterrupt(digitalPinToInterrupt(leftEnc.A), LeftEncoderChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(leftEnc.B), LeftEncoderChange, CHANGE);
  //setup for right encoder
  rightEnc.init();
  attachInterrupt(digitalPinToInterrupt(rightEnc.A), RightEncoderChange, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightEnc.B), RightEncoderChange, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
}

struct EncoderInfo { //struct to return more than one variable from a function.
  int8_t oldAB; 
  int8_t encVal;
};

void LeftEncoderChange(){
  EncoderInfo L;
  L = readEncoder(leftEnc.A, leftEnc.B, leftEnc.oldAB, leftEnc.encVal,leftEnc.leftKey,leftEnc.rightKey);
  leftEnc.oldAB = L.oldAB;
  leftEnc.encVal = L.encVal;
}

void RightEncoderChange(){
  EncoderInfo R;
  R = readEncoder(rightEnc.A, rightEnc.B, rightEnc.oldAB, rightEnc.encVal,rightEnc.leftKey,rightEnc.rightKey);
  rightEnc.oldAB = R.oldAB;
  rightEnc.encVal = R.encVal;
}

struct EncoderInfo readEncoder(int EncA, int EncB, int8_t oldAB, int8_t encVal, char left[], char right[]) {
  struct EncoderInfo info;
  static const int8_t enc_states[]  = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Lookup table

  oldAB <<=2; //remembers previous value(moves bits to left)

  if (digitalRead(EncA)) oldAB |= 0x02; // Add current state of pin A
  if (digitalRead(EncB)) oldAB |= 0x01; // Add current state of pin B
  
  encVal += enc_states[( oldAB & 0x0f )];

  if (encVal < -3){ //turned anti-clockwise
    Keyboard.press(left);
    Keyboard.release(left);
    encVal = 0;
  }
  else if(encVal > 3){ //turned clockwise
    Keyboard.press(right);
    Keyboard.release(right);
    encVal = 0;
  }

  info.oldAB = oldAB;
  info.encVal = encVal;
  return info;
}
