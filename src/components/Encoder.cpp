#include "Encoder.hpp"

Encoder* Encoder::m_instance = nullptr;

Encoder::Encoder(uint8_t pinA, uint8_t pinB) : m_pinA(pinA), m_pinB(pinB) {}

// get singleton instance of the encoder
Encoder* Encoder::getInstance(uint8_t pinA, uint8_t pinB) {
    if (m_instance == nullptr) {
        m_instance = new Encoder(pinA, pinB);
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
        Serial.println("Left");
        m_encVal = 0;
    }
    else if (m_encVal > 3) {  // clockwise
        m_direction = Direction::Right;
        Serial.println("Right");
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
