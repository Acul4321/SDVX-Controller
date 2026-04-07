#include <Arduino.h>
#include <Keyboard.h>

#include "components/Button.hpp"
#include "components/Encoder.hpp"

Button* button = Button::getInstance(4, 'z');
Encoder* encoder = Encoder::getInstance(0, 1, '3', '4');

void setup() {
  Serial.begin(9600);
  Keyboard.begin();
  button->init();
  encoder->init();
}

void loop() {
  button->updateKeyBinding();
  encoder->updateKeyBinding();

  if (button->isPressed()) {
    Serial.println("Button is PRESSED");
  } else {
    Serial.println("Button is RELEASED");
  }

  Encoder::Direction dir = encoder->getDirection();
  if (dir == Encoder::Direction::Left) {
    Serial.println("Encoder turned LEFT");
    encoder->resetDirection();
  } else if (dir == Encoder::Direction::Right) {
    Serial.println("Encoder turned RIGHT");
    encoder->resetDirection();
  }

}