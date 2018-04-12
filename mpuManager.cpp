//
// Created by Jack Thomson on 27/03/2018.
//

#include "mpuManager.h"
#include <thread>
#include <iostream>
#include <fstream>
#include <atomic>

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

void mpuManager::resetAllSensors() {
     for (int i = 0; i != 5; i++) {
        multi.setPath(i);
        Sensor[i].resetSensor();
        Sensor[i].resetStores();
        this_thread::sleep_for(chrono::milliseconds(1));
    }
}

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

void mpuManager::runMultiTest() {
    printf("Running tests\n");

    multi.setPath(8);
    this_thread::sleep_for(chrono::milliseconds(10));
    for (int i = 0; i != 5; i++) {
        multi.setPath(i);
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}

void mpuManager::runInitalisation() {
    printf("Initialising sensors\n\n");
    for (int i = 0; i != 5; i++) {
        multi.setPath(i);
        this_thread::sleep_for(chrono::milliseconds(200));
        Sensor[i].init();
    }
    printf("\n\n");
}

void mpuManager::getDeviceReadings() {
    for (int i = 0; i != 5; i++) {
        multi.setPath(i);
        Sensor[i].storeNewReading();
    }
}

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