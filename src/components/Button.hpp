#include <Arduino.h>

class Button {
public:
    enum class State {
        Up,
        Down
    };

    static Button* getInstance(uint8_t pinNumber, uint8_t keyCode);

    void init();
    State getState();
    bool isPressed();
    bool isReleased();
    void updateKeyBinding();

private:
    explicit Button(uint8_t pinNumber, uint8_t keyCode);
    void updateDebouncedState();

    static Button* m_instance;

    bool m_initialised = false;
    uint8_t m_pin;
    uint8_t m_key;
    bool m_keyHeld = false;
    State m_rawState = State::Up;
    State m_debouncedState = State::Up;
    unsigned long m_lastBounceMs = 0;
    static constexpr unsigned long m_debounceDelayMs = 25;
};