//
// Created by Jack Thomson on 27/03/2018.
//

#include "server.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

server::server() {
    manager.loadAllFiles();
}

void server::startSever() {
    std::cout << "Starting server" << std::endl;

    while(true) {
    }
}
