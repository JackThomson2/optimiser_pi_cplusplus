//
// Created by Jack Thomson on 27/03/2018.
//

#include <array>
#include "sensor.h"
#include "mpuManager.h"
#include <random>

using json = nlohmann::json;

void sensor::storeNewReading() {

    auto sensorVals = getSensorValues();
    auto normalised = getGravityEffect(sensorVals);

    getDistance(normalised);

    xRecordings.emplace_back(xDistance);
    yRecordings.emplace_back(yDistance);
    zRecordings.emplace_back(zDistance);
}

json sensor::getData() {
    return json {
            {"x", xRecordings},
            {"y", yRecordings},
            {"z", zRecordings}
    };
}

void sensor::resetStores() {
    xRecordings = json::array();
    yRecordings = json::array();
    zRecordings = json::array();
}

vector<int16_t> sensor::getSensorValues() {
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int16_t> accelRng(-32767, 32767);
    uniform_int_distribution<int16_t> gyroRng(0, 360);

    auto xAccel = accelRng(rng);
    auto yAccel = accelRng(rng);
    auto zAccel = accelRng(rng);

    auto xGyro = gyroRng(rng);
    auto yGyro = gyroRng(rng);
    auto zGyro = gyroRng(rng);

    return vector<int16_t> {xAccel, yAccel, zAccel, xGyro, yGyro, zGyro};
}

vector<double> sensor::getGravityEffect(vector<int16_t> input) {

    double x = double(input[0]) / double(16384);
    double y = double(input[1]) / double(16384);
    double z = double(input[2]) / double(16384);

    double accel[3] = {x, y, z};
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

    rA[0] = mA[0] * R[0][0] + mA[1] * R[1][0] + mA[2] * R[2][0];
    rA[1] = mA[0] * R[0][1] + mA[1] * R[1][1] + mA[2] * R[2][1];
    rA[2] = mA[0] * R[0][2] + mA[1] * R[1][2] + mA[2] * R[2][2];

    return vector<double> {rA[0], rA[1], rA[2]};
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
