//
// Created by Jack Thomson on 27/03/2018.
//

#ifndef PI_C_SENSOR_H
#define PI_C_SENSOR_H


#include <vector>
using namespace std;

class sensor {
public:
    void storeNewReading();
    vector<vector<int16_t> > getdata();

private:
    vector<vector<int16_t> > recordings;
};


#endif //PI_C_SENSOR_H
