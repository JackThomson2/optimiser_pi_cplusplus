//
// Created by Jack Thomson on 27/03/2018.
//

#include "multiplexer.h"
#include <bcm2835.h>
#include <stdio.h>

multiplexer::multiplexer() {}

void multiplexer::setPath(int path) {
    bcm2835_i2c_setSlaveAddress(I2C_address);
    bcm2835_i2c_begin();
    char pathMessage = path;
    bcm2835_i2c_write(&pathMessage, 1);
    bcm2835_i2c_end();
    return;
}

int multiplexer::getPath() {
    bcm2835_i2c_setSlaveAddress(I2C_address);
    bcm2835_i2c_begin();
    char result;
    bcm2835_i2c_read(&result, 1);
    bcm2835_i2c_end();
    return (int)result;
}