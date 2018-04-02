//
// Created by Jack Thomson on 28/03/2018.
//

#ifndef PI_C_STORAGEMANAGER_H
#define PI_C_STORAGEMANAGER_H

#include "json.hpp"
using json = nlohmann::json;

using namespace std;

const static string storageLocation = "/home/pi/recordings/";
const static int initalLength = storageLocation.length();

class storageManager {
public:
    void loadAllFiles();

    string getFileNamesJson();
    vector<string> getFileNames();
    json getFileByName(string);

    const string searchLocation = storageLocation + "*";
private:
    vector<string> fileNames;
    json fileData;

    json getJsonFromFile(string);

    string getSentName(string);

    string getNameFromRequest(string);
};


#endif //PI_C_STORAGEMANAGER_H
