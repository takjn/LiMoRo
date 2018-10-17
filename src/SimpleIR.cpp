/**
 * SimpleIR.cpp - Simple IR remote control library
 * Copyright (c) 2018 takjn  All right reserved.
 */

#include "SimpleIR.h"

SimpleIR::SimpleIR(PinName rxpin, PinName txpin) : ir_in(rxpin), ir_out(txpin) {
}

SimpleIR::~SimpleIR() {
}

int SimpleIR::receive(unsigned int ir_data[], size_t data_size) {
    unsigned int i;

    timer.reset();
    timer.start();

    for (i = 0; i < data_size; i++) {
        ir_data[i] = 0;
    }

    int now, last, start_at;
    int stat = 0;

    start_at = timer.read_us();
    while (stat = ir_in) {
        if (timer.read_us() - start_at > 1000) {
            return -1;
        }
    }

    last = timer.read_us();
    for (i = 0; i < data_size; i++) {
        while (true) {
            if (stat != ir_in)
                break;
            if (timer.read_us() - last > 100000) {
                if (i < 10) {
                    return -1;
                }
                return i;
            }
        }

        now = timer.read_us();
        ir_data[i] = (now - last);
        last = now;
        stat = (1 - stat);
    }

    return -1;
}

void SimpleIR::transmit(unsigned int *ir_data, size_t data_size) {
    unsigned int i;

    ir_out.period_us(26.3); // 38KHz = 1/38000[s] = 26.315 [us]

    for (i = 0; i < data_size; i++) {
        ir_out.write(0.5 * (1 - (i % 2)));
        wait_us(ir_data[i]);
    }

    ir_out.write(0);
}
