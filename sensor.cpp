//
// Created by Jack Thomson on 27/03/2018.
//

#include <array>
#include "sensor.h"
#include <random>

using json = nlohmann::json;

void sensor::storeNewReading() {

    auto sensorVals = getSensorValues();

    auto normalised = getGravityEffect(sensorVals);

    xRecordings.emplace_back(normalised[0]);
    yRecordings.emplace_back(normalised[1]);
    zRecordings.emplace_back(normalised[2]);
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

vector<float> sensor::getGravityEffect(vector<int16_t> input) {

    float x = input[0] / 16384;
    float y = input[1] / 16384;
    float z = input[2] / 16384;

    float accel[3] = {x, y, z};
    float gravity[3] = {0, 0, 1.0}; // Always vertically downwards at g = 1.0
    float rG[3], rA[3];
    float mA[3];

    // Angle of rotation
    auto alpha = float(input[3] * radConvertor);
    auto beta = float(input[4] * radConvertor);
    auto theta = float(input[5] * radConvertor);

    float R[3][3] =
            {
                    {cos(alpha) * cos(beta), cos(alpha) * sin(beta) * sin(theta) - sin(alpha) * cos(theta),
                                                                                                            cos(alpha) *
                                                                                                            sin(beta) *
                                                                                                            cos(theta) +
                                                                                                            sin(alpha) *
                                                                                                            sin(theta)},
                    {sin(alpha) * cos(beta), sin(alpha) * sin(beta) * sin(theta) + cos(alpha) * cos(theta), sin(alpha) *
                                                                                                            sin(beta) *
                                                                                                            cos(theta) -
                                                                                                            cos(alpha) *
                                                                                                            sin(theta)},
                    {-1 * sin(beta),         cos(beta) * sin(theta),                                        cos(beta) *
                                                                                                            cos(theta)}
            };

    float det = R[0][0] * (R[1][1] * R[2][2] - R[1][2] * R[2][1])\
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

    return vector<float> {rA[0], rA[1], rA[2]};
}
