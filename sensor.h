//
// Created by Jack Thomson on 27/03/2018.
//

#ifndef PI_C_SENSOR_H
#define PI_C_SENSOR_H


#include <vector>
#include "storageManager.h"

using json = nlohmann::json;
using namespace std;

const auto radConvertor = M_PI / 180.0;

class sensor {
public:

    void storeNewReading();

    json getData();

    void resetStores();

private:
    json xRecordings = json::array();
    json yRecordings = json::array();
    json zRecordings = json::array();

    int16_t xGyro = 0;
    int16_t yGyro = 0;
    int16_t zGyro = 0;

    int16_t xAccel = 0;
    int16_t yAccel = 0;
    int16_t zAccel = 0;

    vector<int16_t> getSensorValues();

    vector<float> getGravityEffect(vector<int16_t>);
};


#endif //PI_C_SENSOR_H
