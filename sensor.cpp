//
// Created by Jack Thomson on 27/03/2018.
//

#include <array>
#include "sensor.h"

using json = nlohmann::json;

void sensor::storeNewReading() {
    xRecordings.emplace_back(30);
    yRecordings.emplace_back(20);
    zRecordings.emplace_back(25);
}

json sensor::getData() {
    return json {
            {"x", xRecordings},
            {"y", yRecordings},
            {"z", zRecordings}
    };
}

void sensor::resetStores() {
    xRecordings = json::array();
    yRecordings = json::array();
    zRecordings = json::array();
}
