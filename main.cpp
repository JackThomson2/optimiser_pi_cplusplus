#define MPU6050_INCLUDE_DMP_MOTIONAPPS20

#include "server.h"
#include "MPU6050_6Axis_MotionApps20.h"

//Starts the main bluetooth server
int main() {
    server Server;
    Server.startSever();

    return 0;
}