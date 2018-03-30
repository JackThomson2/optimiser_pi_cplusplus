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

    float xSpeed = 0;
    float ySpeed = 0;
    float zSpeed = 0;

    float xDistance = 0;
    float yDistance = 0;
    float zDistance = 0;


    vector<int16_t> getSensorValues();

    vector<double> getGravityEffect(vector<int16_t>);

    void getDistance(vector<double>);
};


#endif //PI_C_SENSOR_H
