//
// Created by Jack Thomson on 28/03/2018.
//

#ifndef PI_C_STORAGEMANAGER_H
#define PI_C_STORAGEMANAGER_H
#include <nlohmann/json.hpp>
using json = nlohmann::json;

using namespace std;

class storageManager {
public:
    void loadAllFiles();
    json getFileByName(string);
    const char * storageLocation = "store/*";
private:
    vector<string> fileNames;
    vector<json> fileData;
};


#endif //PI_C_STORAGEMANAGER_H
