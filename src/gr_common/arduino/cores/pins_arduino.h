/* pins_arduino.h */
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
#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#ifndef GRLYCHEE
#error "unknown target."
#endif/*GRLYCHEE*/

#define NUM_DIGITAL_PINS            40
#define NUM_ANALOG_INPUTS           6

/** IO pins, header CN10. */
#define PIN_IO0     0 // P5_14(RXD4)
#define PIN_IO1     1 // P5_15(TXD4)
#define PIN_IO2     2 // P5_9
#define PIN_IO3     3 // P5_8
#define PIN_IO4     4 // P5_11
#define PIN_IO5     5 // P5_10
#define PIN_IO6     6 // P2_1
#define PIN_IO7     7 // P2_0

/** IO pins, header CN9. */
#define PIN_IO8     8 // P3_10
#define PIN_IO9     9 // P3_9
#define PIN_IO10   10 // P6_13
#define PIN_IO11   11 // P6_14
#define PIN_IO12   12 // P6_15
#define PIN_IO13   13 // P6_12
#define PIN_IO14   14 // P1_15 -> same as A0
#define PIN_IO15   15 // P1_14 -> same as A1

#define PIN_I2C_SDA 20 // P1_7
#define PIN_I2C_SCL 21 // P1_6

/** USB TX and RX. */
#define PIN_USBTX  28 // P3_15(TXD1)
#define PIN_USBRX  29 // P0_2(RXD1)

/** LEDs (D1-D4) and SW. */
#define PIN_LED1   22 // LED1
#define PIN_LED2   23 // LED2
#define PIN_LED3   24 // LED3
#define PIN_LED4   25 // LED4
#define PIN_SW0    26 // USER_BUTTON0
#define PIN_SW1    27 // USER_BUTTON1
#define PIN_LED_RED     PIN_LED4
#define PIN_LED_ORANGE  PIN_LED3
#define PIN_LED_YELLOW  PIN_LED2
#define PIN_LED_GREEN   PIN_LED1
#define PIN_USER_BUTTON0 PIN_SW0
#define PIN_USER_BUTTON1 PIN_SW1

/** */
#define PIN_ESP_TX  30 // P7_1(TXD2)
#define PIN_ESP_RX  31 // P0_1(RXD2)
#define PIN_ESP_IO0 32 // P3_14
#define PIN_ESP_IO2 33 // P3_13
#define PIN_ESP_EN  34 // P5_3
#define PIN_ESP_RTS 35 // P7_6
#define PIN_ESP_CTS 36 // P7_7
#define PIN_ESP_IO15 37 // P5_13
#define PIN_IO38     38 // P4_0
#define PIN_IO39     39 // P4_1


// ANALOG IO PINS -------------------------------------------------------------/

/** Analog pins, header CN15. */
#define PIN_AN000     14 // P1_15
#define PIN_AN001     15 // P1_14
#define PIN_AN002     16 // P1_13
#define PIN_AN003     17 // P1_12
#define PIN_AN004     18 // P1_11
#define PIN_AN005     19 // P1_10

// I2C PINS -------------------------------------------------------------------/

/** I2C pins, header CN15. */
static const uint8_t SDA = PIN_I2C_SDA;
static const uint8_t SCL = PIN_I2C_SCL;

// SPI PINS -------------------------------------------------------------------/

/** SPI pins, header CN9. */
static const uint8_t SS   = PIN_IO10;
static const uint8_t MOSI = PIN_IO11;
static const uint8_t MISO = PIN_IO12;
static const uint8_t SCK  = PIN_IO13;

#endif/*Pins_Arduino_h*/
