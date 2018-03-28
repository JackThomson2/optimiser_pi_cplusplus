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
    sensor Sensor[6];
    int WAIT_TIME = 50;


    void getDeviceReadings();

    void storeJSON();

    void initRecording();
};


#endif //PI_C_MPUMANAGER_H
