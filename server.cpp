//
// Created by Jack Thomson on 27/03/2018.
//

#include "server.h"
#include "I2Cdev.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

using namespace std;

server::server() {
    //INIT CODE
}

void server::startSever() {
    printf("Setting up i2c\n");
    I2Cdev::initialize();
    printf("Initialized i2c\n");
    sensorManager.runInitalisation();

    printf("Starting server\n");
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    char address[18] = "B8:27:EB:D9:30:C6";
    socklen_t opt = sizeof(rem_addr);

    printf("Waiting for connection...\n\n");
    while (true) {
        // allocate socket
        s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

        // bind socket to port 1 of the first available 
        // local bluetooth adapter
        loc_addr.rc_family = AF_BLUETOOTH;
        str2ba( address, &loc_addr.rc_bdaddr);
        loc_addr.rc_channel = (uint8_t) 1;
        bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

        // put socket into listening mode
        listen(s, 1);

        // accept one connection
        client = accept(s, (struct sockaddr *)&rem_addr, &opt);
        ba2str( &rem_addr.rc_bdaddr, buf );
        fprintf(stderr, "Accepted connection from %s\n\n", buf);

        while (client) {
            memset(buf, 0, sizeof(buf));

            // read data from the client
            bytes_read = read(client, buf, sizeof(buf));
            string message;
            printf("Recieved %s\n", buf);
            if( bytes_read > 0 ) {
                message = checkRequest(buf);
                if (message == "stop") {
                    close(client);
                    close(s);
                    return;
                }

                printf("Sending response...\n");
                auto status = write(client, message.c_str(), message.length());
                if (status < 500 && status > 0)
                    printf("Sent %s, status %i\n\n", message.c_str(), status);
                else
                    printf("Sent big message, size %i\n\n", status);

            } else {
                close(client);
                close(s);
                client = false;  
                continue;
            }
        }

        printf("Client disconnected waiting for new item\n\n\n");
    }

    // close connection
    close(client);
    close(s);
}

string server::checkRequest(const string &request) {
    if (request == "q") {
        return "stop";
    } else if (request == "transaction:start") {
        startRecording();
        return "true";
    } else if (request == "transaction:stop") {
        stopRecording();
        return "true";
    } else if (request == "transaction:list") {
        return getFiles();
    } else if (request.find("transaction:init:filename:") != string::npos) {
        return getFile(request);
    } else {
        printf("Sorry %s is an unknown command\n\n", request.c_str());
    }
    return "";
}

string server::getFile(string request) {
    auto requestLength = request.length();
    auto fileName = request.substr(26, requestLength - 30);

    return storage.getFileByName(fileName).dump();
}

string server::getFiles() {
   return storage.getFileNamesJson();
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
