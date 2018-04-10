//
// Created by Jack Thomson on 27/03/2018.
//

#ifndef PI_C_MULTIPLEXER_H
#define PI_C_MULTIPLEXER_H

#define i2c_baudrate 500000

#define I2C_address     0x70
#define I2C_bus_number  1
#define I2C_ch_0        0x02
#define I2C_ch_1        0x03
#define I2C_ch_2        0x04
#define I2C_ch_3        0x05
#define I2C_ch_4        0x06
#define I2C_ch_5        0x07
#define I2C_ch_6        0x08
#define I2C_ch_7        0x09

#include "I2Cdev.h"

class multiplexer {
public:
    multiplexer();
    void setPath(int);
    
    int getPath();
private:

    uint8_t buffer[14];
};

#endif //PI_C_MULTIPLEXER_H
