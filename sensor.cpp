//
// Created by Jack Thomson on 27/03/2018.
//

#include <array>
#include "sensor.h"

void sensor::storeNewReading() {
    vector<int16_t> record = {30, 40, 50};

    recordings.push_back(record);
}

vector<vector<int16_t>> sensor::getdata() {
    return recordings;
}
