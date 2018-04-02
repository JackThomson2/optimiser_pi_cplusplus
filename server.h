//
// Created by Jack Thomson on 27/03/2018.
//

#ifndef PI_C_SERVER_H
#define PI_C_SERVER_H

#include <thread>
#include <atomic>
#include "storageManager.h"
#include "mpuManager.h"

using namespace std;

class server {
public:
    server();
    void startSever();
private:
    storageManager storage;
    thread recordingThread;
    atomic<bool> running{false};
    mpuManager sensorManager;

    bool checkRequest(const string &);

    void printFiles();

    void startRecording();

    void stopRecording();
};


#endif //PI_C_SERVER_H
