//
// Created by Jack Thomson on 28/03/2018.
//

#include "storageManager.h"
#include <glob.h>
#include <fstream>

using namespace std;

// Used to load all the files from the specified directory
// stores them all in the file names variable
void storageManager::loadAllFiles() {
    fileNames = {};
    glob_t glob_result{};
    glob(searchLocation.c_str(), GLOB_TILDE, nullptr, &glob_result);
    printf("Searching for files..\n\n");

    for (unsigned int i = 0; i < glob_result.gl_pathc; ++i)
        fileNames.emplace_back(glob_result.gl_pathv[i]);

}

// Used as a function get getting a record by name
// if an item is not already cached will request a load
json storageManager::getFileByName(string name) {
    name = getNameFromRequest(name);

    if (fileData[name] == nullptr)
        fileData[name] = getJsonFromFile(name);

    return fileData[name];
}

// Used to get the file names and return them as JSON
// data is converted to Javascript time
string storageManager::getFileNamesJson() {
    loadAllFiles();
    json info = json::array();

    for (auto file : fileNames) {
        info.emplace_back(getSentName(file) + "000");
    }

    return info.dump();
}

// Used to load all the file names and return the vector
vector<string> storageManager::getFileNames() {
    loadAllFiles();

    return fileNames;
}


// Used to load json directly from a file
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

// Used to get teh name used for sending
string storageManager::getSentName(string requestWord) {
    int totalLength = requestWord.length();
    int endLoc = totalLength - initalLength - 7;
    return requestWord.substr(initalLength, endLoc);
}

// Appends the file type to a file from a request
string storageManager::getNameFromRequest(string requestWord) {
    return storageLocation + requestWord + ".record";
}