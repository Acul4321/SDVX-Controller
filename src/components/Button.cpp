#include "Button.hpp"

Button* Button::m_instance = nullptr;

Button::Button(uint8_t pinNumber) : m_pin(pinNumber) {}

// get singleton instance of the button
Button* Button::getInstance(uint8_t pinNumber) {
    if (m_instance == nullptr) {
        m_instance = new Button(pinNumber);
    }
    return m_instance;
}


// init button pin if not init already
void Button::init() {
    if (!m_initialised) {
        pinMode(m_pin, INPUT_PULLUP);
        m_initialised = true;
    }
}

Button::State Button::getState() {
    return digitalRead(m_pin) == LOW ? State::Down : State::Up;
}

bool Button::isPressed() {
    return getState() == State::Down;
}

bool Button::isReleased() {
    return getState() == State::Up;
}