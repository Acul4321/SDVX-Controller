#include "Encoder.hpp"
#include <Keyboard.h>

Encoder* Encoder::m_instance = nullptr;

Encoder::Encoder(uint8_t pinA, uint8_t pinB, uint8_t leftKey, uint8_t rightKey)
    : m_pinA(pinA), m_pinB(pinB), m_leftKey(leftKey), m_rightKey(rightKey) {}

// get singleton instance of the encoder
Encoder* Encoder::getInstance(uint8_t pinA, uint8_t pinB, uint8_t leftKey, uint8_t rightKey) {
    if (m_instance == nullptr) {
        m_instance = new Encoder(pinA, pinB, leftKey, rightKey);
    }
    return m_instance;
}

// init the encoder if not been init already
void Encoder::init() {
    if (!m_initialised) {
        pinMode(m_pinA, INPUT_PULLUP);
        pinMode(m_pinB, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(m_pinA), onEncoderChange, CHANGE);
        attachInterrupt(digitalPinToInterrupt(m_pinB), onEncoderChange, CHANGE);
        m_initialised = true;
    }
}

// static interrupt handler, calls the instance method
void Encoder::onEncoderChange() {
    if (m_instance != nullptr) {
        m_instance->handleEncoderChange();
    }
}

// instance method to handle encoder state changes
void Encoder::handleEncoderChange() {
    // lookup table for encoder state transitions from: https://www.eevblog.com/forum/projects/rotary-encoder-jocks-a-question-about-the-lookup-table/
    static const int8_t enc_states[] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

    m_oldAB <<= 2;  // remember previous value

    // Add current state of pins
    if (digitalRead(m_pinA)) m_oldAB |= 0x02;
    if (digitalRead(m_pinB)) m_oldAB |= 0x01;

    m_encVal += enc_states[(m_oldAB & 0x0f)];

    // direction based on accumulated value
    if (m_encVal < -3) {  // counter-clockwise
        m_direction = Direction::Left;
        m_leftPending = true;
        m_encVal = 0;
    }
    else if (m_encVal > 3) {  // clockwise
        m_direction = Direction::Right;
        m_rightPending = true;
        m_encVal = 0;
    }
    else { // stoped
        m_direction = Direction::Idle;
    }
}

Encoder::Direction Encoder::getDirection() {
    return m_direction;
}

// reset direction to Idle
void Encoder::resetDirection() {
    m_direction = Direction::Idle;
}

void Encoder::updateKeyBinding() {
    bool sendLeft = false;
    bool sendRight = false;

    noInterrupts();
    if (m_leftPending) {
        sendLeft = true;
        m_leftPending = false;
    }
    if (m_rightPending) {
        sendRight = true;
        m_rightPending = false;
    }
    interrupts();

    if (sendLeft) {
        Keyboard.press(m_leftKey);
        Keyboard.release(m_leftKey);
    }
    if (sendRight) {
        Keyboard.press(m_rightKey);
        Keyboard.release(m_rightKey);
    }
}
