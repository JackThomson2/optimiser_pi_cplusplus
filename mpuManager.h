//
// Created by Jack Thomson on 27/03/2018.
//

#ifndef PI_C_MPUMANAGER_H
#define PI_C_MPUMANAGER_H

#include "sensor.h"

const int WAIT_TIME = 50;
const double RECORDS_PER_SECOND = double(WAIT_TIME) / 1000;

class mpuManager {
public:
    void startRecording(std::atomic<bool>&);
private:
    sensor Sensor[6];

    void getDeviceReadings();

    void storeJSON();

    void initRecording();
};


#endif //PI_C_MPUMANAGER_H
