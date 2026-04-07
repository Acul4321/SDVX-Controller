#include "Button.hpp"
#include <Keyboard.h>

Button* Button::m_instance = nullptr;

Button::Button(uint8_t pinNumber, uint8_t keyCode) : m_pin(pinNumber), m_key(keyCode) {}

// get singleton instance of the button
Button* Button::getInstance(uint8_t pinNumber, uint8_t keyCode) {
    if (m_instance == nullptr) {
        m_instance = new Button(pinNumber, keyCode);
    }
    return m_instance;
}


// init button pin if not init already
void Button::init() {
    if (!m_initialised) {
        pinMode(m_pin, INPUT_PULLUP);
        const State initialState = (digitalRead(m_pin) == LOW) ? State::Down : State::Up;
        m_rawState = initialState;
        m_debouncedState = initialState;
        m_lastBounceMs = millis();
        m_initialised = true;
    }
}

Button::State Button::getState() {
    updateDebouncedState();
    return m_debouncedState;
}

bool Button::isPressed() {
    return getState() == State::Down;
}

bool Button::isReleased() {
    return getState() == State::Up;
}

void Button::updateDebouncedState() {
    const State currentRaw = (digitalRead(m_pin) == LOW) ? State::Down : State::Up;
    const unsigned long now = millis();

    if (currentRaw != m_rawState) {
        m_rawState = currentRaw;
        m_lastBounceMs = now;
    }

    if ((now - m_lastBounceMs) >= m_debounceDelayMs) {
        m_debouncedState = m_rawState;
    }
}

void Button::updateKeyBinding() {
    const bool pressed = isPressed();

    if (pressed && !m_keyHeld) {
        Keyboard.press(m_key);
        m_keyHeld = true;
    } else if (!pressed && m_keyHeld) {
        Keyboard.release(m_key);
        m_keyHeld = false;
    }
}