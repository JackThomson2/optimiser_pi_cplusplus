//
// Created by Jack Thomson on 27/03/2018.
//

#ifndef PI_C_SENSOR_H
#define PI_C_SENSOR_H


#include <vector>
#include "storageManager.h"

using json = nlohmann::json;
using namespace std;

class sensor {
public:

    void storeNewReading();

    json getData();

    void resetStores();

private:
    json xRecordings = json::array();
    json yRecordings = json::array();
    json zRecordings = json::array();
};


#endif //PI_C_SENSOR_H
