#include <Arduino.h>
#include <Keyboard.h>

#include "components/Button.hpp"
#include "components/Encoder.hpp"

Encoder* VOL_L = Encoder::getInstance(0, 1, '3', '4');
Encoder* VOL_R = Encoder::getInstance(2, 3, '0', '-');

Button* BT_A = Button::getInstance(4, 'e');
Button* BT_B = Button::getInstance(5, 'r');
Button* BT_C = Button::getInstance(6, 'o');
Button* BT_D = Button::getInstance(7, 'p');

Button* FX_L = Button::getInstance(8, 'v');
Button* FX_R = Button::getInstance(9, ',');

void setup() {
  Serial.begin(9600);
  Keyboard.begin();

  BT_A->init();
  BT_B->init();
  BT_C->init();
  BT_D->init();
  FX_L->init();
  FX_R->init();
  VOL_L->init();
  VOL_R->init();
}

void loop() {
  BT_A->updateKeyBinding();
  BT_B->updateKeyBinding();
  BT_C->updateKeyBinding();
  BT_D->updateKeyBinding();

  VOL_L->updateKeyBinding();
  VOL_R->updateKeyBinding();
  FX_L->updateKeyBinding();
  FX_R->updateKeyBinding();
}