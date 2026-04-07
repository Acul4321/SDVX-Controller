#include <Arduino.h>
#include "components/Button.hpp"
#include "components/Encoder.hpp"

Button* button = Button::getInstance(4);
Encoder* encoder = Encoder::getInstance(0, 1);

void setup() {
  Serial.begin(9600);
  button->init();
  encoder->init();
}

void loop() {
  if (button->isPressed()) {
    Serial.println("Button is PRESSED");
  } else {
    Serial.println("Button is RELEASED");
  
  Encoder::Direction dir = encoder->getDirection();
  if (dir == Encoder::Direction::Left) {
    Serial.println("Encoder turned LEFT");
    encoder->resetDirection();
  } else if (dir == Encoder::Direction::Right) {
    Serial.println("Encoder turned RIGHT");
    encoder->resetDirection();
  }
  
  }
  delay(500);
}