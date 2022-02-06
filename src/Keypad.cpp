#include "Keypad.h"
#include <Arduino.h>
#include "HID-Project.h"

const char Keypad::_keys[10][4] = {
    { '.', '/', '\0', '\0' },   { 'a', 'b', 'c', '\0' }, { 'd', 'e', 'f', '\0' },
    { 'g', 'h', 'i', '\0' },    { 'j', 'k', 'l', '\0' }, { 'm', 'n', 'o', '\0' },
    { 'p', 'q', 'r', 's' },     { 't', 'u', 'v', '\0' }, { 'w', 'x', 'y', 'z' },
                                                    { ' ', '*', '\0', '\0' }
};

Keypad::Keypad() : _lastAction(-1), _repeats(0), _lastPress(0), _shifted(false) {}

void Keypad::pressKey(uint16_t key) {
    if (key == 0xF) {
        _shifted = !_shifted;
        return;
    }

    if (key != _lastAction || _keys[key][_repeats] == '\0' || millis() - _lastPress >= timeout || _repeats > 3) _repeats = 0;
    if (millis() - _lastPress < timeout && _lastAction == key) {
        BootKeyboard.write(KEY_BACKSPACE);
    }

    _lastAction = key;
    _lastPress = millis();
    char character = _keys[key][_repeats++];
    if (_shifted && key > 0 && key < 9) character -= 32;
    BootKeyboard.write(character);
}

