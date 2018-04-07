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
    initRecording();

    while(stop.load()) {
        auto start_s = chrono::steady_clock::now();
        getDeviceReadings();
        auto diff = chrono::steady_clock::now() - start_s;

        while((chrono::duration<double , milli> (diff).count()) < WAIT_TIME) {
            this_thread::sleep_for(chrono::milliseconds(1));
            diff = chrono::steady_clock::now() - start_s;
        }
    }
    storeJSON();

    printf("I got %lu recordings.\n", Sensor[0].getData()["ax"].size());
}

void mpuManager::runMultiTest() {
    printf("Running tests\n");
    for (int i = 1; i != 6; i++) {
        multi.setPath(i);
        printf("Reading %i ", i);
        printf("Multi reads %i\n\n", multi.getPath());
    }
}

void mpuManager::runInitalisation() {
    printf("Initialising sensors\n");
    for (int i = 1; i != 6; i++) {
        multi.setPath(i);
        this_thread::sleep_for(chrono::milliseconds(200));
        Sensor[i].init();
    }
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

void mpuManager::initRecording() {
    I2Cdev::initialize();
    printf("Initialized i2c\n");
    for (int i = 0; i != 5; i++)
        Sensor[i].resetStores();
}