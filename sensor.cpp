//
// Created by Jack Thomson on 27/03/2018.
//

#include <array>
#include "sensor.h"
#include "mpuManager.h"

#include <thread>

using json = nlohmann::json;

void sensor::init() {
    accelgyro.initialize();
    this_thread::sleep_for(chrono::milliseconds(200));
    auto info = accelgyro.dmpInitialize();
    this_thread::sleep_for(chrono::milliseconds(200));
    accelgyro.setDMPEnabled(true);
    this_thread::sleep_for(chrono::milliseconds(200));
    packetSize = accelgyro.dmpGetFIFOPacketSize();
    accelgyro.setDLPFMode(1);

    printf("Connection good %s\n", accelgyro.testConnection() ? "Yes" : "No");
}

void sensor::storeNewReading(bool record) {
    while (true) {
        fifocount = accelgyro.getFIFOCount();
        while (fifocount < packetSize)
            fifocount = accelgyro.getFIFOCount();

        if (fifocount == 1024) {
            accelgyro.resetFIFO();
            printf("WARNING HAD TO RESET!\n\n\n");
        } else {
            uint8_t toTake = floor(fifocount / packetSize);
            toTake *= packetSize;

            accelgyro.getFIFOBytes(fifobuffer, toTake);
            bufferDump.insert(bufferDump.end(), fifobuffer, fifobuffer + toTake);
            cntr += toTake;
            return;
        }
    }
}

void sensor::resetSensor() {
    accelgyro.resetFIFO();
}

json sensor::getData() {
    processData();

    printf("Size %i\n", axRecordings.size());
    return json {
            {"x", axRecordings},
            {"y", ayRecordings},
            {"z", azRecordings}
    };
}

void sensor::resetStores() {
    axRecordings = json::array();
    ayRecordings = json::array();
    azRecordings = json::array();
    
    cntr = 0;
    bufferDump.clear();
}

void sensor::getDistance(vector<double> accelerations) {
    double xSpeedChange = accelerations[0] * RECORDS_PER_SECOND;
    double ySpeedChange = accelerations[1] * RECORDS_PER_SECOND;
    double zSpeedChange = accelerations[2] * RECORDS_PER_SECOND;

    xSpeed += xSpeedChange;
    ySpeed += ySpeedChange;
    zSpeed += zSpeedChange;

    xDistance += xSpeedChange * RECORDS_PER_SECOND;
    yDistance += ySpeedChange * RECORDS_PER_SECOND;
    zDistance += zSpeedChange * RECORDS_PER_SECOND;
}

void sensor::processData() {
    for (int i = 0; i < bufferDump.size(); i += packetSize) {
        for (int x = 0; x != packetSize; x++)
            fifobuffer[x] = bufferDump[x + i];
            
        accelgyro.dmpGetQuaternion(&q, fifobuffer);
        accelgyro.dmpGetAccel(&aa, fifobuffer);
        accelgyro.dmpGetGravity(&gravity, &q);
        accelgyro.dmpGetLinearAccel(&aaReal, &aa, &gravity);    
        accelgyro.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);

        axRecordings.emplace_back(aaWorld.x);
        ayRecordings.emplace_back(aaWorld.y);
        azRecordings.emplace_back(aaWorld.z);
    }
}
