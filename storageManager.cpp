//
// Created by Jack Thomson on 28/03/2018.
//

#include "storageManager.h"
#include <glob.h>

using namespace std;

void storageManager::loadAllFiles() {
    glob_t glob_result{};
    glob(storageLocation.c_str(), GLOB_TILDE, nullptr, &glob_result);
    printf("Searching for files\n\n");

    for(unsigned int i=0; i < glob_result.gl_pathc; ++i){
        fileNames.emplace_back(glob_result.gl_pathv[i]);
    }
}

json storageManager::getFileByName(string) {
    return json();
}

vector<string> storageManager::getFileNames() {
    return fileNames;
}
