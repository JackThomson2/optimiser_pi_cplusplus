//
// Created by Jack Thomson on 27/03/2018.
//

#include "multiplexer.h"
#include <bcm2835.h>
#include <stdio.h>
#include "I2Cdev.h"

multiplexer::multiplexer() {}

void multiplexer::setPath(int path) {
    uint8_t config = 0x00;
    config |= (0x01 << path);
    I2Cdev::writeByte(I2C_address, 0x04, 0x00);

    I2Cdev::writeByte(I2C_address, 0x04, config);
}

int multiplexer::getPath() {
    bcm2835_i2c_setSlaveAddress(I2C_address);
    char result;
    bcm2835_i2c_read(&result, 1);
    return (int)result;
}