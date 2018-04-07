//
// Created by Jack Thomson on 27/03/2018.
//

#include <array>
#include "sensor.h"
#include "mpuManager.h"
#include <random>

using json = nlohmann::json;

void sensor::init() {
    accelgyro.initialize();
}

void sensor::storeNewReading() {

    auto sensorVals = getSensorValues();
    auto normalised = getGravityEffect(sensorVals);

    xDistance += normalised[3];
    yDistance += normalised[4];
    zDistance += normalised[5];

    //getDistance(normalised);

    axRecordings.emplace_back(normalised[0]);
    ayRecordings.emplace_back(normalised[1]);
    azRecordings.emplace_back(normalised[2]);

    gxRecordings.emplace_back(normalised[3]);
    gyRecordings.emplace_back(normalised[4]);
    gzRecordings.emplace_back(normalised[5]);
}

json sensor::getData() {
    printf("\nX change %d Y change %d Z change %d\n", xDistance, yDistance, zDistance);
    return json {
            {"ax", axRecordings},
            {"ay", ayRecordings},
            {"az", azRecordings},
            {"gx", gxRecordings},
            {"gy", gyRecordings},
            {"gz", gzRecordings}
    };
}

void sensor::resetStores() {
    axRecordings = json::array();
    ayRecordings = json::array();
    azRecordings = json::array();

    gxRecordings = json::array();
    gyRecordings = json::array();
    gzRecordings = json::array();
}

vector<int16_t> sensor::getSensorValues() {
    int16_t xAccel, yAccel, zAccel;
    int16_t xGyro, yGyro, zGyro;

    accelgyro.getMotion6(&xAccel, &yAccel, &zAccel, &xGyro, &yGyro, &zGyro);

    return vector<int16_t> {xAccel, yAccel, zAccel, xGyro, yGyro, zGyro};
}

vector<double> sensor::getGravityEffect(vector<int16_t> input) {

    double ax = double(input[0]) / sensorOffset;
    double ay = double(input[1]) / sensorOffset;
    double az = double(input[2]) / sensorOffset;

    double gx = double(input[3]) / gyroOffset;
    double gy = double(input[4]) / gyroOffset;
    double gz = double(input[5]) / gyroOffset;

    double accel[3] = {ax, ay, az};
    double gravity[3] = {0, 0, 1.0}; // Always vertically downwards at g = 1.0
    double rG[3], rA[3];
    double mA[3];

    // Angle of rotation
    auto alpha = input[3] * radConvertor;
    auto beta = input[4] * radConvertor;
    auto theta = input[5] * radConvertor;

    double R[3][3] =
            {
                    {cos(alpha) * cos(beta), cos(alpha) * sin(beta) * sin(theta) - sin(alpha) * cos(theta),
                                                                     cos(alpha) * sin(beta) * cos(theta) +
                                                                     sin(alpha) * sin(theta)},

                    {sin(alpha) * cos(beta), sin(alpha) * sin(beta) * sin(theta) + cos(alpha) * cos(theta),
                                                                     sin(alpha) * sin(beta) * cos(theta) -
                                                                     cos(alpha) * sin(theta)},

                    {-1 * sin(beta),         cos(beta) * sin(theta), cos(beta) * cos(theta)}
            };


    double det = R[0][0] * (R[1][1] * R[2][2] - R[1][2] * R[2][1])\
 - R[0][1] * (R[1][0] * R[2][2] - R[1][2] * R[2][0])\
 + R[0][2] * (R[1][0] * R[2][1] - R[1][1] * R[2][0]);

    rG[0] = gravity[0] * R[0][0] + gravity[1] * R[0][1] + gravity[2] * R[0][2];
    rG[1] = gravity[0] * R[1][0] + gravity[1] * R[1][1] + gravity[2] * R[1][2];
    rG[2] = gravity[0] * R[2][0] + gravity[1] * R[2][1] + gravity[2] * R[2][2];

    rA[0] = accel[0] * R[0][0] + accel[1] * R[0][1] + accel[2] * R[0][2];
    rA[1] = accel[0] * R[1][0] + accel[1] * R[1][1] + accel[2] * R[1][2];
    rA[2] = accel[0] * R[2][0] + accel[1] * R[2][1] + accel[2] * R[2][2];

    mA[0] = rA[0] - rG[0];
    mA[1] = rA[1] - rG[1];
    mA[2] = rA[2] - rG[2];

    rA[0] = (mA[0] * R[0][0] + mA[1] * R[1][0] + mA[2] * R[2][0]) * gravityAccel;
    rA[1] = (mA[0] * R[0][1] + mA[1] * R[1][1] + mA[2] * R[2][1]) * gravityAccel;
    rA[2] = (mA[0] * R[0][2] + mA[1] * R[1][2] + mA[2] * R[2][2]) * gravityAccel;

    return vector<double> {ax, ay, az, gx, gy, gz};
}

void sensor::getDistance(vector<double> accelerations) {
    double xSpeedChange = accelerations[0] * RECORDS_PER_SECOND;
    double ySpeedChange = accelerations[1] * RECORDS_PER_SECOND;
    double zSpeedChange = accelerations[2] * RECORDS_PER_SECOND;

    xSpeed += xSpeedChange;
    ySpeed += ySpeedChange;
    zSpeed += zSpeedChange;

    xDistance += xSpeedChange * RECORDS_PER_SECOND;
    yDistance += ySpeedChange * RECORDS_PER_SECOND;
    zDistance += zSpeedChange * RECORDS_PER_SECOND;
}
