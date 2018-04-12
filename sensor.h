//
// Created by Jack Thomson on 27/03/2018.
//

#ifndef PI_C_SENSOR_H
#define PI_C_SENSOR_H

#include <vector>
#include "storageManager.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "helper_3dmath.h"

using json = nlohmann::json;
using namespace std;

class sensor {
public:
    void init();

    void storeNewReading(bool = true);

    json getData();

    void resetSensor();

    void resetStores();

private:
    json axRecordings = json::array();
    json ayRecordings = json::array();
    json azRecordings = json::array();
    json stats = {};

    const double OFFSETS = 16384.0;
    const double GRAVITY = 9.807;
    const double TIMEMULTIPLIER = 1.0 / 100.0;

    int cntr = 0;

    MPU6050 accelgyro;

    uint16_t fifocount;
    uint8_t status;
    uint16_t packetSize;
    uint8_t fifobuffer[1024];

    vector<uint8_t > bufferDump;

    Quaternion q;
    VectorInt16 aa;
    VectorInt16 aaReal;
    VectorInt16 aaWorld;
    VectorFloat gravity;

    float eurler[3];
    float ypr[3];

    void processData();
};


#endif //PI_C_SENSOR_H
