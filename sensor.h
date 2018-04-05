//
// Created by Jack Thomson on 27/03/2018.
//

#ifndef PI_C_SENSOR_H
#define PI_C_SENSOR_H

#include <vector>
#include "storageManager.h"
#include "I2Cdev.h"
#include "MPU6050.h"

using json = nlohmann::json;
using namespace std;

const auto radConvertor = M_PI / 180.0;
const double gravityAccel = 9.84016;
const double sensorOffset = 16384.0;

class sensor {
public:
    void init();

    void storeNewReading();

    json getData();

    void resetStores();

private:
    json axRecordings = json::array();
    json ayRecordings = json::array();
    json azRecordings = json::array();

    json gxRecordings = json::array();
    json gyRecordings = json::array();
    json gzRecordings = json::array();

    float xSpeed = 0;
    float ySpeed = 0;
    float zSpeed = 0;

    float xDistance = 0;
    float yDistance = 0;
    float zDistance = 0;

    MPU6050 accelgyro;

    vector<int16_t> getSensorValues();

    vector<double> getGravityEffect(vector<int16_t>);

    void getDistance(vector<double>);
};


#endif //PI_C_SENSOR_H
