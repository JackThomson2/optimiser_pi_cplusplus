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

    printf("Connection good %s init success %i\n", accelgyro.testConnection() ? "Yes" : "No", info);
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
            {"z", azRecordings},
            {"stats", stats}
    };
}

void sensor::resetStores() {
    axRecordings = json::array();
    ayRecordings = json::array();
    azRecordings = json::array();
    
    cntr = 0;
    bufferDump.clear();
}

void sensor::processData() {
    double xSpeed = 0.0;
    double ySpeed = 0.0;
    double zSpeed = 0.0;

    double xDistance = 0.0;
    double yDistance = 0.0;
    double zDistance = 0.0;

    double xMin = numeric_limits<double>::max();
    double yMin = numeric_limits<double>::max();
    double zMin = numeric_limits<double>::max();

    double xMax = numeric_limits<double>::min();
    double yMax = numeric_limits<double>::min();
    double zMax = numeric_limits<double>::min();

    for (int i = 0; i < bufferDump.size(); i += packetSize) {
        for (int x = 0; x != packetSize; x++)
            fifobuffer[x] = bufferDump[x + i];
            
        accelgyro.dmpGetQuaternion(&q, fifobuffer);
        accelgyro.dmpGetAccel(&aa, fifobuffer);
        accelgyro.dmpGetGravity(&gravity, &q);
        accelgyro.dmpGetLinearAccel(&aaReal, &aa, &gravity);    
        accelgyro.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);

        double x = (double(aaWorld.x) / OFFSETS) * GRAVITY;
        double y = (double(aaWorld.y) / OFFSETS) * GRAVITY;
        double z = (double(aaWorld.z) / OFFSETS) * GRAVITY;

        xSpeed += x * TIMEMULTIPLIER;
        ySpeed += y * TIMEMULTIPLIER;
        zSpeed += z * TIMEMULTIPLIER;

        xDistance += xSpeed * TIMEMULTIPLIER;
        yDistance += ySpeed * TIMEMULTIPLIER;
        zDistance += zSpeed * TIMEMULTIPLIER;

        if (xSpeed > xMax)
            xMax = xSpeed;
        
        if (xSpeed < xMin)
            xMin = xSpeed;

        if (ySpeed > yMax)
            yMax = ySpeed;
        
        if (ySpeed < yMin)
            yMin = ySpeed;

        if (zSpeed > zMax)
            zMax = zSpeed;
        
        if (zSpeed < zMin)
            xMin = xSpeed;        

        axRecordings.emplace_back(x);
        ayRecordings.emplace_back(y);
        azRecordings.emplace_back(z);
    }
    stats = {
        {"xDistance", xDistance},
        {"yDistance", yDistance},
        {"zDistance", zDistance},

        {"xMax", xMax},
        {"yMax", yMax},
        {"zMax", zMax},

        {"xMin", xMin},
        {"yMin", yMin},
        {"zMin", zMin}
    };
}
