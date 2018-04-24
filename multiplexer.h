//
// Created by Jack Thomson on 27/03/2018.
//

#ifndef PI_C_MULTIPLEXER_H
#define PI_C_MULTIPLEXER_H

#define i2c_baudrate 500000

#define I2C_address     0x70
#define I2C_bus_number  1


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
