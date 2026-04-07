#pragma once

#include <Arduino.h>

class Encoder {
public:
    enum class Direction {
        Idle,
        Left,
        Right
    };

    static Encoder* getInstance(uint8_t pinA, uint8_t pinB, uint8_t leftKey, uint8_t rightKey);

    void init();
    Direction getDirection();
    void resetDirection();
    void updateKeyBinding();

private:
    explicit Encoder(uint8_t pinA, uint8_t pinB, uint8_t leftKey, uint8_t rightKey);

    static Encoder* m_instance;

    bool m_initialised = false;
    uint8_t m_pinA;
    uint8_t m_pinB;
    uint8_t m_leftKey;
    uint8_t m_rightKey;
    int8_t m_oldAB = 3;  // 0000 0011 default starting value
    int8_t m_encVal = 0;
    volatile Direction m_direction = Direction::Idle;
    volatile bool m_leftPending = false;
    volatile bool m_rightPending = false;

    // Static interrupt handler
    static void onEncoderChange();

    // Instance method called by interrupt
    void handleEncoderChange();
};
