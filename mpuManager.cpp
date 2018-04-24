//
// Created by Jack Thomson on 27/03/2018.
//

#include "mpuManager.h"
#include <thread>
#include <iostream>
#include <fstream>
#include "catch.hpp"

using namespace std;

// With a loop will tell all the sensors to record their information
void mpuManager::startRecording(atomic<bool>& stop) {
    resetAllSensors();
    zeroGyros();

    resetAllSensors();
    printf("Gyros have been zero'ed now recording\n\n");

    while(stop.load()) {
        getDeviceReadings();
    }
    storeJSON();
    resetAllSensors();
    printf("I got %lu recordings.\n", Sensor[0].getData()["x"].size());
}

// This is a quick loop for resetting all sensors
void mpuManager::resetAllSensors() {
    for (int i = 0; i != 5; i++) {
        multi.setPath(i);
        Sensor[i].resetSensor();
        Sensor[i].resetStores();
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}

// This is a the 10 second wait which takes place as the gyroscope zero themselves
void mpuManager::zeroGyros() {
    auto start_s = chrono::steady_clock::now();
    while (true) {
        for (int i = 0; i != 5; i++) {
            multi.setPath(i);
            Sensor[i].storeNewReading();
        }
        auto diff = chrono::steady_clock::now() - start_s;
        if ((chrono::duration<double , milli> (diff).count()) > 10000)
            return;
    }
}

// Quick test to see if the multiplexer is running as expected
bool mpuManager::runMultiTest() {
    printf("Running tests\n");

    multi.setPath(8);
    this_thread::sleep_for(chrono::milliseconds(10));
    for (int i = 0; i != 5; i++) {
        multi.setPath(i);
        this_thread::sleep_for(chrono::milliseconds(10));
        uint8_t expected = 0x00;
        expected |= (0x01 << i);
        if (expected != multi.getPath())
            return false;
    }

    return true;
}

// Loop for making sure all the sensors are setup
void mpuManager::runInitialisation() {
    printf("Initialising sensors\n\n");
    for (int i = 0; i != 5; i++) {
        multi.setPath(i);
        this_thread::sleep_for(chrono::milliseconds(200));
        Sensor[i].init();
    }
    printf("\n\n");
}

// This loop runs through all the sensors and runs the function getting them to take a new recording
void mpuManager::getDeviceReadings() {
    for (int i = 0; i != 5; i++) {
        multi.setPath(i);
        Sensor[i].storeNewReading();
    }
}

//Used to store the data directly from the sensors
void mpuManager::storeJSON() {
    json returnJson = json::array();

    for (int i = 0; i != 5; i++) {
        json stored = {to_string(i), Sensor[i].getData()};
        returnJson.emplace_back(stored);
    }

    string fileName = storageLocation + std::to_string(time(0)) + ".record";

    ofstream recording;
    recording.open(fileName, ios::out | ios::trunc);
    recording << returnJson.dump().c_str();
    recording.close();

    printf("Saved to file to \"%s\".\n", fileName.c_str());
}

TEST_CASE("Checking manager", "[mpuManager]") {

    // Check that all the tests run successfully
    SECTION(" check that sensors initialise properly ") {
        mpuManager mpu;

        mpu.runInitialisation();
    }

    // Check that all the tests run successfully
    SECTION(" check that multiplexer tests work as expected ") {
        mpuManager mpu;

        REQUIRE(mpu.runMultiTest());
    }

    // Test that I can create a new recording and the number of items increases
    SECTION(" check that creating a new recording increases the number ") {
        mpuManager mpu;
        storageManager store;

        unsigned long initialSize = store.getFileNames().size();
        atomic<bool> running{true};

        auto recordingThread = thread(&mpuManager::startRecording, mpu, ref(running));
        this_thread::sleep_for(chrono::milliseconds(100));

        running = false;
        recordingThread.join();

        REQUIRE(store.getFileNames().size() > initialSize);
    }

}