#ifndef IR_KEYPAD_H
#define IR_KEYPAD_H

#include <stdint.h>
#include <time.h>

class Keypad {
public:
    Keypad();

    void pressKey(uint16_t key);

private:
    uint16_t _lastAction;
    uint8_t _repeats;
    unsigned long _lastPress;
    bool _shifted;
    static const char _keys[10][4];
    static const int timeout = 1000;
};


#endif //IR_KEYPAD_H
