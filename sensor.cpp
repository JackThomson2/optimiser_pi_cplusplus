//
// Created by Jack Thomson on 27/03/2018.
//

#include <array>
#include "sensor.h"
#include "mpuManager.h"
#include <random>

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
    printf("Connection good %s\n", accelgyro.testConnection() ? "Yes" : "No");
    printf("Packet Size %i \n\n",packetSize);
}

void sensor::storeNewReading(bool record) {
    while(true) {
        fifocount = accelgyro.getFIFOCount();
        while (fifocount < packetSize) 
            fifocount = accelgyro.getFIFOCount();

        status = accelgyro.getIntStatus();

        if (fifocount == 1024) {
            accelgyro.resetFIFO();
            printf("WARNING HAD TO RESET!\n\n\n");
        } else if (status & 0x02) {
            uint8_t toTake = floor(fifocount / packetSize);
            toTake *= packetSize;
            
            accelgyro.getFIFOBytes(fifobuffer, toTake);
            cntr += toTake;

            /*
            accelgyro.dmpGetQuaternion(&q, fifobuffer);
            accelgyro.dmpGetAccel(&aa, fifobuffer);
            accelgyro.dmpGetGravity(&gravity, &q);
            accelgyro.dmpGetLinearAccel(&aaReal, &aa, &gravity);    
            accelgyro.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);

            if (record == true) {
                axRecordings.emplace_back(aaWorld.x);
                ayRecordings.emplace_back(aaWorld.y);
                azRecordings.emplace_back(aaWorld.z);
            } */

            bufferDump.insert(bufferDump.end(), fifobuffer, fifobuffer + toTake);
            
            //printf("Count was %i took %i items.\n", fifocount, toGet);
            return;
        }
    }
}

void sensor::resetSensor() {
    accelgyro.resetFIFO();
}

json sensor::getData() {
    return json {
            {"ax", axRecordings},
            {"ay", ayRecordings},
            {"az", azRecordings}
    };
}

void sensor::resetStores() {
    axRecordings = json::array();
    ayRecordings = json::array();
    azRecordings = json::array();

    printf("Size %i counter %i\n", bufferDump.size(), cntr);
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
