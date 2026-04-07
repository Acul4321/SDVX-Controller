#pragma once

#include <Arduino.h>

class Encoder {
public:
    enum class Direction {
        Idle,
        Left,
        Right
    };

    static Encoder* getInstance(uint8_t pinA, uint8_t pinB);

    void init();
    Direction getDirection();
    void resetDirection();

private:
    explicit Encoder(uint8_t pinA, uint8_t pinB);

    static Encoder* m_instance;

    bool m_initialised = false;
    uint8_t m_pinA;
    uint8_t m_pinB;
    int8_t m_oldAB = 3;  // 0000 0011 default starting value
    int8_t m_encVal = 0;
    Direction m_direction = Direction::Idle;

    // Static interrupt handler
    static void onEncoderChange();

    // Instance method called by interrupt
    void handleEncoderChange();
};
