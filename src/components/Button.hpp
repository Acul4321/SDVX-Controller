#include <Arduino.h>

class Button {
public:
    enum class State {
        Up,
        Down
    };

    static Button* getInstance(uint8_t pinNumber);

    void init();
    State getState();
    bool isPressed();
    bool isReleased();

private:
    explicit Button(uint8_t pinNumber);

    static Button* m_instance;

    bool m_initialised = false;
    uint8_t m_pin;
};