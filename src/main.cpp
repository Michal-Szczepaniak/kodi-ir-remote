#include <Arduino.h>
#include "HID-Project.h"
#include "Keypad.h"
#include <IRremote.hpp>
#include <StandardCplusplus.h>
#include <vector>
#include <string>
#include <sstream>
#include "PCA9685.h"
#include <Wire.h>

// 0064 - ir code
Keypad keypad;
PCA9685 pwmController;
unsigned long timeout = 0;

void setup() {
    Wire.begin();
    Serial.begin(115200);
    IrReceiver.begin(A2);
    BootKeyboard.begin();
    pinMode(5, INPUT);
    pinMode(4, OUTPUT);

    pwmController.resetDevices();
    pwmController.init();
    pwmController.setPWMFrequency(1526);
    pwmController.setChannelPWM(0, 2900);
}

void loop() {
    while (Serial.available()) {
        String input = Serial.readStringUntil('\r');

        std::vector<double> levels;
        std::istringstream f(input.c_str());
        std::string s;

        for (char c : input) {
            if (c == ',') {
                levels.push_back(strtod(s.c_str(), nullptr));
                s.clear();
            } else {
                s.push_back(c);
            }
        }

        if (!s.empty()) {
            levels.push_back(strtod(s.c_str(), nullptr));
        }

        if (levels.empty()) break;

        bool peaking = (bool)levels.back();
        levels.pop_back();

        digitalWrite(4,peaking);

        for (unsigned int i = 0; i < levels.size(); i++) {
            pwmController.setChannelPWM(i, (levels[i]*29.));
        }
    }

    if (IrReceiver.decode()) {
//        Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        if (millis() - timeout > 200) {
            timeout = millis();
//            Serial.println(IrReceiver.decodedIRData.decodedRawData);
            switch (IrReceiver.decodedIRData.decodedRawData) {
                case 0xBB448672:
//                    Serial.println("Channel+");
                    break;
                case 0xC99E0576:
//                    Serial.println("Channel-");
                    break;
                case 0x8BC2B5F1:
//                    Serial.println("Volume+");
                    BootKeyboard.write(KEY_VOLUME_UP);
                    break;
                case 0x8C01DDCD:
//                    Serial.println("Volume+");
                    BootKeyboard.write(KEY_VOLUME_DOWN);
                    break;
                case 0x95B671D6:
//                    Serial.println("Web");
                    BootKeyboard.write(KEY_MENU);
                    break;
                case 0x8F71AFF2:
//                    Serial.println("Menu");
                    BootKeyboard.write(KEY_TAB);
                    break;
                case 0xE5DB652D:
//                    Serial.println("Back");
                    BootKeyboard.write(KEY_BACKSPACE);
                    break;
                case 0xA0B11D6:
//                    Serial.println("Subtitles");
                    BootKeyboard.write(KEY_L);
                    break;
                case 0xEA20242D:
//                    Serial.println("Audio");
                    BootKeyboard.press(KEY_LEFT_CTRL);
                    BootKeyboard.press(KEY_A);
                    BootKeyboard.releaseAll();
                    break;
                case 0x84CCD056:
//                    Serial.println("Search");
                    BootKeyboard.write(KEY_I);
                    break;
                case 0xFBB192D2:
//                    Serial.println("Very Up");
                    BootKeyboard.write(KEY_PAGE_UP);
                    break;
                case 0x6B8FD5B2:
//                    Serial.println("Very Down");
                    BootKeyboard.write(KEY_PAGE_DOWN);
                    break;
                case 0xA54EEF0D:
//                    Serial.println("Up");
                    BootKeyboard.write(KEY_UP);
                    break;
                case 0x2A8D308D:
//                    Serial.println("Down");
                    BootKeyboard.write(KEY_DOWN);
                    break;
                case 0x1C33B189:
//                    Serial.println("Left");
                    BootKeyboard.write(KEY_LEFT);
                    break;
                case 0x51C3CB2D:
//                    Serial.println("Right");
                    BootKeyboard.write(KEY_RIGHT);
                    break;
                case 0x93264F5A:
//                    Serial.println("Enter");
                    BootKeyboard.write(KEY_ENTER);
                    break;
                case 0x95E3B8F5:
//                    Serial.println("Seek backward");
                    BootKeyboard.write(KEY_R);
                    break;
                case 0xD626D892:
//                    Serial.println("Play");
                    BootKeyboard.write(KEY_P);
                    break;
                case 0x6B40973A:
//                    Serial.println("Pause");
                    BootKeyboard.write(KEY_SPACE);
                    break;
                case 0x1F936D12:
//                    Serial.println("Seek forward");
                    BootKeyboard.write(KEY_F);
                    break;
                case 0xA43D37F9:
//                    Serial.println("Stop");
                    BootKeyboard.write(KEY_X);
                    break;
                case 0xDA958455:
//                    Serial.println("Mute");
                    BootKeyboard.write(KEY_MUTE);
                    break;
                case 0x9F60FE01:
//                    Serial.println("Movies");
                    BootKeyboard.press(KEY_LEFT_CTRL);
                    BootKeyboard.press(KEY_E);
                    BootKeyboard.releaseAll();
                    break;
                case 0xBA5CE9FA:
//                    Serial.println("F1");
                    BootKeyboard.press(KEY_LEFT_CTRL);
                    BootKeyboard.press(KEY_1);
                    BootKeyboard.releaseAll();
                    break;
                case 0x611B2792:
//                    Serial.println("F2");
                    BootKeyboard.write(KEY_F2);
                    break;
                case 0x69FE358E:
//                    Serial.println("F3");
                    BootKeyboard.write(KEY_F3);
                    break;
                case 0x9998E1D2:
//                    Serial.println("F4");
                    BootKeyboard.press(KEY_LEFT_ALT);
                    BootKeyboard.press(KEY_F4);
                    BootKeyboard.releaseAll();
                    break;
                case 0xF8F84D2:
//                    Serial.println("On");
                    pinMode(5, OUTPUT);
                    delay(100);
                    pinMode(5, INPUT);
                    break;
                case 0x1767AF71:
                    keypad.pressKey(0x0);
                    break;
                case 0x318A151:
                    keypad.pressKey(0x1);
                    break;
                case 0xFCA418D1:
                    keypad.pressKey(0x2);
                    break;
                case 0xAFD97D5:
                    keypad.pressKey(0x3);
                    break;
                case 0xE7AF0F51:
                    keypad.pressKey(0x4);
                    break;
                case 0x474E8371:
                    keypad.pressKey(0x5);
                    break;
                case 0xB21EF5AD:
                    keypad.pressKey(0x6);
                    break;
                case 0xC07874B1:
                    keypad.pressKey(0x7);
                    break;
                case 0x55B9A80D:
                    keypad.pressKey(0x8);
                    break;
                case 0x9C584175:
                    keypad.pressKey(0x9);
                    break;
                case 0x55A99171:
                    keypad.pressKey(0xF);
                    break;
//                default:
//                    IrReceiver.printIRResultShort(&Serial);
            }
        }
        IrReceiver.resume();
    }
}
