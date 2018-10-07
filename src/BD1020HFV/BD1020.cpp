/*****************************************************************************
  BD1020.cpp

 Copyright (c) 2016 ROHM Co.,Ltd.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
******************************************************************************/
#include <avr/pgmspace.h>
#include <Arduino.h>
#include <BD1020.h>

BD1020::BD1020(void)
{

}

void BD1020::init(PinName ana_pin)
{
  _analog_pin  = ana_pin;
}

void BD1020::get_rawval(unsigned short *data)
{
  *data = analogRead(_analog_pin);
}

void BD1020::get_val(float *data)
{
  get_rawval(&temp_adc);
  *data = convert_degree(temp_adc);
}

// Calculations for Temp Sensor - BD1020
// Math:  ADC_Voltage = sensorValue * (5V/1024)
//        Tempurature = - (1000 * ADC_Voltage - 1546) / 8.2
float BD1020::convert_degree(unsigned short data)
{
  float tmp, temp;

  tmp = (float)data * 5 / 1024;
  temp = -(1000 * tmp - 1546) / 8.2;

  return (temp);
}
