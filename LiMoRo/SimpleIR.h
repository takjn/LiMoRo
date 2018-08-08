/**
 * SimpleIR.h - Simple IR remote control library
 * Copyright (c) 2018 takjn  All right reserved.
 */

#ifndef _SIMPLE_IR_H_
#define _SIMPLE_IR_H_

#include <mbed.h>

class SimpleIR
{
  public:
    explicit SimpleIR(PinName rxpin, PinName txpin);
    ~SimpleIR();

    int receive(unsigned int *ir_data, size_t data_size);
    void transmit(unsigned int *ir_data, size_t data_size);

  private:
    DigitalIn ir_in;
    PwmOut ir_out;
    Timer timer;
};
#endif
