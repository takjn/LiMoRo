/* wiring_private.cpp */
/* Copyright (C) 2016 Nozomu Fujita, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <wiring_private.h>
#include <pins_arduino.h>
#include <mbed.h>
extern Timer arduino_system_timer;

int get_millis_arduino_system_timer(void)
{
    return arduino_system_timer.read_ms();
}

int get_micros_arduino_system_timer(void)
{
    return arduino_system_timer.read_us();
}

// Pin number→PinName Conversion table
const PinName PinNames[NUM_DIGITAL_PINS] = {
		P5_14, P5_15, P5_9, P5_8, P5_11, P5_10, P2_1, P2_0, P3_10, P3_9,
		P6_13, P6_14, P6_15, P6_12, P1_15, P1_14, P1_13, P1_12, P1_11, P1_10,
		P1_7, P1_6, LED1, LED2, LED3, LED4, USER_BUTTON0, USER_BUTTON1, P3_15, P0_2,
		P7_1, P0_1, P3_14, P3_13, P5_3, P7_7, P7_6, P5_13, P4_0, P4_1
};

/*
 One troublesome aspect of the Arduino library is that pin initialization and 
 completion processing are handled separately. 
 As a solution, we have unified the interface for pin initialization and 
 completion processing in the library.
 Using “ArduinoPinModeXXXX” as a symbol to indicate the mode for each pin operation, 
 you can implement initialization function “setArduinoPinModeXXXX(PinName pinName)”
 and completion “resetArduinoPinModeXXXX(PinName pinName)” function as needed.
 Use the following to set a pin operation to a specific mode:

 setArduinoPinMode(PinName pinName, ArduinoPinMode mode)

 If the pin is already initialized to a different mode, function
 “resetArduinoPinModeXXXX(PinName pinName)” 
 will be called to end that mode, and then function setArduinoPinModeXXXX(PinName pinName)
 will be called to initialize the pin to the desired mode.
*/

// Pin Status Table
ArduinoPinStatus arduinoPinStatus[MaxPinName + 1] = {
    {ArduinoPinModeUnused},
};

void setArduinoPinMode(PinName pinName, ArduinoPinMode mode)
{
    if (isValidPinName(pinName)) {
        if (getArduinoPinMode(pinName) != mode) {
            resetArduinoPinMode(pinName);
            switch (mode) {
            case ArduinoPinModeUnused:
                if (setArduinoPinModeUnused != NULL) {
                    setArduinoPinModeUnused(pinName);
                }
                goto change;
            case ArduinoPinModeInput:
                if (setArduinoPinModeInput != NULL) {
                    setArduinoPinModeInput(pinName);
                }
                goto change;
            case ArduinoPinModeOutput:
                if (setArduinoPinModeOutput != NULL) {
                    setArduinoPinModeOutput(pinName);
                }
                goto change;
            case ArduinoPinModeInputPullUp:
                if (setArduinoPinModeInputPullUp != NULL) {
                    setArduinoPinModeInputPullUp(pinName);
                }
                goto change;
            case ArduinoPinModeOutputOpenDrain:
                if (setArduinoPinModeOutputOpenDrain != NULL) {
                    setArduinoPinModeOutputOpenDrain(pinName);
                }
                goto change;
            case ArduinoPinModeAnalogRead:
                if (setArduinoPinModeAnalogRead != NULL) {
                    setArduinoPinModeAnalogRead(pinName);
                }
                goto change;
            case ArduinoPinModeHardwarePWM:
                if (setArduinoPinModeHardwarePWM != NULL) {
                    setArduinoPinModeHardwarePWM(pinName);
                }
                goto change;
            case ArduinoPinModeSoftwarePWM:
                if (setArduinoPinModeSoftwarePWM != NULL) {
                    setArduinoPinModeSoftwarePWM(pinName);
                }
                goto change;
            case ArduinoPinModeInterrupt:
                if (setArduinoPinModeInterrupt != NULL) {
                    setArduinoPinModeInterrupt(pinName);
                }
                goto change;
            case ArduinoPinModeTone:
                if (setArduinoPinModeTone != NULL) {
                    setArduinoPinModeTone(pinName);
                }
                goto change;
            case ArduinoPinModeServo:
                if (setArduinoPinModeServo != NULL) {
                    setArduinoPinModeServo(pinName);
                }
                goto change;
            case ArduinoPinModeDac:
                if (setArduinoPinModeDac != NULL) {
                    setArduinoPinModeDac(pinName);
                }
                goto change;
            case ArduinoPinModeOther:
                if (setArduinoPinModeOther != NULL) {
                    setArduinoPinModeOther(pinName);
                }
              change:
                changeArduinoPinMode(pinName, mode);
                break;
            default:
                break;
            }
        }
    }
}

ArduinoPinMode getArduinoPinMode(PinName pinName)
{
    return isValidPinName(pinName) ? arduinoPinStatus[pinName].mode : ArduinoPinModeError;
}

void changeArduinoPinMode(PinName pinName, ArduinoPinMode mode)
{
    if (isValidPinName(pinName)) {
        arduinoPinStatus[pinName].mode = mode;
    }
}

void resetArduinoPinMode(PinName pinName)
{
    if (isValidPinName(pinName)) {
        ArduinoPinMode mode = getArduinoPinMode(pinName);
        switch (mode) {
        case ArduinoPinModeUnused:
            break;
        case ArduinoPinModeInput:
            if (resetArduinoPinModeInput != NULL) {
                resetArduinoPinModeInput(pinName);
            }
            goto change;
        case ArduinoPinModeOutput:
            if (resetArduinoPinModeOutput != NULL) {
                resetArduinoPinModeOutput(pinName);
            }
            goto change;
        case ArduinoPinModeInputPullUp:
            if (resetArduinoPinModeInputPullUp != NULL) {
                resetArduinoPinModeInputPullUp(pinName);
            }
            goto change;
        case ArduinoPinModeOutputOpenDrain:
            if (resetArduinoPinModeOutputOpenDrain != NULL) {
                resetArduinoPinModeOutputOpenDrain(pinName);
            }
            goto change;
        case ArduinoPinModeAnalogRead:
            if (resetArduinoPinModeAnalogRead != NULL) {
                resetArduinoPinModeAnalogRead(pinName);
            }
            goto change;
        case ArduinoPinModeHardwarePWM:
            if (resetArduinoPinModeHardwarePWM != NULL) {
                resetArduinoPinModeHardwarePWM(pinName);
            }
            goto change;
        case ArduinoPinModeSoftwarePWM:
            if (resetArduinoPinModeSoftwarePWM != NULL) {
                resetArduinoPinModeSoftwarePWM(pinName);
            }
            goto change;
        case ArduinoPinModeInterrupt:
            if (resetArduinoPinModeInterrupt != NULL) {
                resetArduinoPinModeInterrupt(pinName);
            }
            goto change;
        case ArduinoPinModeTone:
            if (resetArduinoPinModeTone != NULL) {
                resetArduinoPinModeTone(pinName);
            }
            goto change;
        case ArduinoPinModeServo:
            if (resetArduinoPinModeServo != NULL) {
                resetArduinoPinModeServo(pinName);
            }
            goto change;
        case ArduinoPinModeDac:
            if (resetArduinoPinModeDac != NULL) {
                resetArduinoPinModeDac(pinName);
            }
            goto change;
        case ArduinoPinModeOther:
            if (resetArduinoPinModeOther != NULL) {
                resetArduinoPinModeOther(pinName);
            }
          change:
            changeArduinoPinMode(pinName, ArduinoPinModeUnused);
            break;
        default:
            break;
        }
    }
}
