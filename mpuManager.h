//
// Created by Jack Thomson on 27/03/2018.
//

#ifndef PI_C_MPUMANAGER_H
#define PI_C_MPUMANAGER_H

#include "sensor.h"
#include "I2Cdev.h"
#include "multiplexer.h"

const int WAIT_TIME = 1;
const double RECORDS_PER_SECOND = 1000 / double(WAIT_TIME);

class mpuManager {
public:
    void startRecording(std::atomic<bool>&);

    bool runMultiTest();

    void runInitialisation();
private:
    sensor Sensor[6];

    multiplexer multi;

    void getDeviceReadings();

    void storeJSON();

    void zeroGyros();

    void resetAllSensors();

    void initRecording();
};


#endif //PI_C_MPUMANAGER_H
