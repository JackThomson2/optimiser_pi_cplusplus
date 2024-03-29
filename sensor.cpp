//
// Created by Jack Thomson on 27/03/2018.
//

#include <array>
#include "sensor.h"
#include "catch.hpp"

#include <thread>

using json = nlohmann::json;

// This initialises the accelerometer ready for reading
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

// This is used read the cache from the sensors and dump into the buffer to save time
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

//This is called to reset the FIFO of the sensor
void sensor::resetSensor() {
    accelgyro.resetFIFO();
}

///This function is called to get a wrapped up object of data
json sensor::getData() {
    processData();

    printf("Size %i\n", axRecordings.size());
    return json {
            {"x",     axRecordings},
            {"y",     ayRecordings},
            {"z",     azRecordings},
            {"stats", stats}
    };
}

// This clears all the stores into fresh state
void sensor::resetStores() {
    axRecordings = json::array();
    ayRecordings = json::array();
    azRecordings = json::array();

    cntr = 0;
    bufferDump.clear();
}

// This parses the quickly dumped data into usable information
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

            {"xMax",      xMax},
            {"yMax",      yMax},
            {"zMax",      zMax},

            {"xMin",      xMin},
            {"yMin",      yMin},
            {"zMin",      zMin}
    };
}

// This function is used to find a variance in data in a specific region
double sensor::getVarience(vector<double> input, int size, bool absolute, bool max) {
    if (input.size() < size)
        return 0;

    double target = max ? numeric_limits<double>::min() : numeric_limits<double>::max();

    for (int i = 0; i < input.size() - size; i += size) {
        vector<double> subset = vector<double >(input.begin() + i, input.begin() + size);
        auto range = getRange(subset, absolute);

        if (max && range > target)
            target = max;
        else if (!max && range < target)
            target = max;
    }

    return target;
}

// Used to calculate the range of input vector,
double sensor::getRange(vector<double> input, bool absolute) {
    if (input.size() < 1)
        return 0.0;

    double difference = 0;
    double last = input[0];

    for (const double &value : input) {
        difference += absolute ? abs(last - value) : value - last;
        last = value;
    }

    return difference;
}

/*
TEST_CASE("Checking sensor", "[sensor]") {

    // Check that all the tests run successfully
    SECTION(" check that the sensor is reading and the json data is full ") {
        sensor snsr;

        snsr.init();
        snsr.storeNewReading();

        auto res = snsr.getData();

        REQUIRE(!res["x"].empty());
        REQUIRE(!res["y"].empty());
        REQUIRE(!res["z"].empty());

        REQUIRE(!res["stats"].empty());
    }

    // Check that all the tests run successfully
    SECTION(" check that the sensor data is clearing correctly ") {
        sensor snsr;

        snsr.init();
        snsr.storeNewReading();
        snsr.resetStores();

        auto res = snsr.getData();

        REQUIRE(res["x"].empty());
        REQUIRE(res["y"].empty());
        REQUIRE(res["z"].empty());

        REQUIRE(res["stats"].empty());
    }
}*/