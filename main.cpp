#define MPU6050_INCLUDE_DMP_MOTIONAPPS20
#define CATCH_CONFIG_MAIN

#include "server.h"

//Starts the main bluetooth server
int main() {
    server Server;
    Server.startSever();

    return 0;
}