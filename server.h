//
// Created by Jack Thomson on 27/03/2018.
//

#ifndef PI_C_SERVER_H
#define PI_C_SERVER_H

#include "storageManager.h"

class server {
public:
    server();
    void startSever();
private:
    storageManager manager;
};


#endif //PI_C_SERVER_H
