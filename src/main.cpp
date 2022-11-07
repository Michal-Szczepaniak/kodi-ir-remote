#include <Arduino.h>
#include "HID-Project.h"
#include "Keypad.h"

#define DECODE_SONY

#include <IRremote.hpp>

/*
 * 0x13 Channel+
 * 0x14 Channel-
 * 0x1B Menu
 * 0x1A Channel level
 * 0xE Return
 * 0x53 Search
 * 0x79 Up
 * 0x7A Down
 * 0x7B Left
 * 0x7C Right
 * 0xB Enter
 * 0x33 Seek backward
 * 0x32 Play
 * 0x34 Seek forward
 * 0x93A Prev
 * 0x39 Pause
 * 0x38 Stop
 * 0x31 Next
 * 0x2E On
 * 0x2F Off
 */
Keypad keypad;

void setup() {
    Serial.begin(115200);
    IrReceiver.begin(A2);
    BootKeyboard.begin();
    pinMode(5, INPUT);
}

void loop() {
    if (IrReceiver.decode()) {
        if (IrReceiver.decodedIRData.flags ^ IRDATA_FLAGS_WAS_OVERFLOW && IrReceiver.decodedIRData.flags ^ IRDATA_FLAGS_IS_REPEAT && IrReceiver.decodedIRData.protocol == SONY) {
            switch (IrReceiver.decodedIRData.command) {
                case 0x13:
                    Serial.println("Channel+");
                    break;
                case 0x14:
                    Serial.println("Channel-");
                    BootKeyboard.press(KEY_LEFT_CTRL);
                    BootKeyboard.press(KEY_1);
                    BootKeyboard.releaseAll();
                    break;
                case 0x1B:
                    Serial.println("Menu");
                    BootKeyboard.write(KEY_MENU);
                    break;
                case 0x1A:
                    Serial.println("Channel level");
                    BootKeyboard.write(KEY_TAB);
                    break;
                case 0xE:
                    Serial.println("Return");
                    BootKeyboard.write(KEY_BACKSPACE);
                    break;
                case 0x53:
                    Serial.println("Search");
                    BootKeyboard.write(KEY_I);
                    break;
                case 0x79:
                    Serial.println("Up");
                    BootKeyboard.write(KEY_UP);
                    break;
                case 0x7A:
                    Serial.println("Down");
                    BootKeyboard.write(KEY_DOWN);
                    break;
                case 0x7B:
                    Serial.println("Left");
                    BootKeyboard.write(KEY_LEFT);
                    break;
                case 0x7C:
                    Serial.println("Right");
                    BootKeyboard.write(KEY_RIGHT);
                    break;
                case 0xB:
                    Serial.println("Enter");
                    BootKeyboard.write(KEY_ENTER);
                    break;
                case 0x33:
                    Serial.println("Seek backward");
                    BootKeyboard.write(KEY_R);
                    break;
                case 0x32:
                    Serial.println("Play");
                    BootKeyboard.write(KEY_P);
                    break;
                case 0x34:
                    Serial.println("Seek forward");
                    BootKeyboard.write(KEY_F);
                    break;
                case 0x30:
                    Serial.println("Prev");
                    BootKeyboard.write(KEY_PAGE_DOWN);
                    break;
                case 0x39:
                    Serial.println("Pause");
                    BootKeyboard.write(KEY_SPACE);
                    break;
                case 0x38:
                    Serial.println("Stop");
                    BootKeyboard.write(KEY_X);
                    break;
                case 0x31:
                    Serial.println("Next");
                    BootKeyboard.write(KEY_UP);
                    break;
                case 0x2E:
                    Serial.println("On");
                    pinMode(4, OUTPUT);
                    delay(100);
                    pinMode(4, INPUT);
                    break;
                case 0x2F:
                    Serial.println("Off");
                    BootKeyboard.write(KEY_S);
                    break;
                case 0x0:
                case 0x1:
                case 0x2:
                case 0x3:
                case 0x4:
                case 0x5:
                case 0x6:
                case 0x7:
                case 0x8:
                case 0x9:
                case 0xF:
                    keypad.pressKey(IrReceiver.decodedIRData.command);
                    break;
                default:
                    IrReceiver.printIRResultShort(&Serial);
            }
        }
        IrReceiver.resume();
    }
}