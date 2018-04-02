//
// Created by Jack Thomson on 28/03/2018.
//

#include "storageManager.h"
#include <glob.h>
#include <fstream>

using namespace std;

void storageManager::loadAllFiles() {
    fileNames = {};
    glob_t glob_result{};
    glob(searchLocation.c_str(), GLOB_TILDE, nullptr, &glob_result);
    printf("Searching for files..\n\n");

    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i)
        fileNames.emplace_back(glob_result.gl_pathv[i]);

}

json storageManager::getFileByName(string name) {
    name = getNameFromRequest(name);

    if (fileData[name] == nullptr)
        fileData[name] = getJsonFromFile(name);

    return fileData[name];
}

string storageManager::getFileNamesJson() {
    loadAllFiles();
    json info = json::array();

    for (auto file : fileNames) {
        info.emplace_back(getSentName(file) + "000");
    }

    return info.dump();
}

vector<string> storageManager::getFileNames() {
    loadAllFiles();
    
    return fileNames;
}

json storageManager::getJsonFromFile(string location) {
    ifstream file(location);
    if (!file.good() || !file.is_open())
        return {};

    string line;
    string result;

    while (getline(file, line))
        result += line;

    file.close();

    return json::parse(result);
}

string storageManager::getSentName(string requestWord) {
    int totalLength = requestWord.length();
    int endLoc = totalLength - initalLength - 7;
    return requestWord.substr(initalLength, endLoc);
}

string storageManager::getNameFromRequest(string requestWord) {
    return storageLocation + requestWord + ".record";
}