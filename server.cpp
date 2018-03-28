//
// Created by Jack Thomson on 27/03/2018.
//

#include "server.h"
#include <iostream>

using namespace std;

server::server() {
    storageManager.loadAllFiles();
}

void server::startSever() {
    cout << "Starting server" << endl;
    string read;

    while(true) {
        cin >> read;

        if (!checkRequest(read))
            return;
    }
}


bool server::checkRequest(const string &request) {
    if (request == "q") {
        return false;
    } else if (request == "print") {
        printFiles();
    } else if (request == "record") {
        startRecording();
    } else if (request == "stop") {
        stopRecording();
    } else {
        printf("Sorry %s is an unknown command\n\n", request.c_str());
    }
    return true;
}

void server::printFiles() {
    printf("Files recorded:\n");
    for (auto const &c : storageManager.getFileNames())
        printf("%s\n", c.c_str());
}

void server::startRecording() {
    if (running)
        return;

    running = true;
    recordingThread = thread(&mpuManager::startRecording, sensorManager, ref(running));
    printf("Recording...\n");
}

void server::stopRecording() {
    if (!running)
        return;

    printf("Halting thread...\n");
    running = false;
    recordingThread.join();
    printf("Thread stopped.\n");
}


