#include <iostream>
#include "server.h"
#include <atomic>
#include <thread>
#include "mpuManager.h"

//Starts the main bluetooth server
int main() {
    mpuManager manager;
    std::atomic<bool> running {true };

    thread t1(&mpuManager::startRecording, manager, std::ref(running));

    this_thread::sleep_for(std::chrono::seconds(10));

    running = false;
    t1.join();

    return 0;
}