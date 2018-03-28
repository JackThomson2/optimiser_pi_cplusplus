//
// Created by Jack Thomson on 27/03/2018.
//

#include "mpuManager.h"
#include <chrono>
#include <thread>

using namespace std;

// With a loop will tell all the sensors to record their information
void mpuManager::startRecording(std::atomic<bool>& stop) {

    while(stop) {
        auto start_s = chrono::steady_clock::now();
        getDevices();
        auto diff = chrono::steady_clock::now() - start_s;

        while((chrono::duration<double , milli> (diff).count()) < WAIT_TIME) {
            this_thread::sleep_for(chrono::milliseconds(1));
            diff = chrono::steady_clock::now() - start_s;
        }
    }

    printf("I got %lu recordings\n",Sensor[0].getdata().size());
}


void mpuManager::getDevices() {
    for (int i = 0; i != 5; i++) {
        Sensor[i].storeNewReading();
    }
}


