//
// Created by Jack Thomson on 27/03/2018.
//

#ifndef PI_C_MPUMANAGER_H
#define PI_C_MPUMANAGER_H

#include "sensor.h"


class mpuManager {
public:
    void startRecording(std::atomic<bool>&);
private:
    void getDevices();
    sensor Sensor[6];
    int RECORDS_PER_SECOND = 20;
    float SECONDS_PER_RECORD = 1 / RECORDS_PER_SECOND;
    int WAIT_TIME = 50;
};


#endif //PI_C_MPUMANAGER_H
