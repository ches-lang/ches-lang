/* 
 * 
 * Chestnut 0.0.0
 * 
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 * 
 * Copyright © 2020 Garnet3106 All rights reserved. 
 * 
 */


#pragma once

#include <vector>
#include <unordered_map>
#include <vector>

#include "../filemanager/filemanager.hpp"

#include "./settings.cpp"


ches::Settings::Settings() {}


// excep: FileError
void ches::Settings::load(std::string filePath) {
    std::vector<std::string> lineVec;

    try {
        lineVec = FileManager::readTextLines(filePath);
    } catch(FileError excep) {
        throw excep;
    }

    for(std::string line : lineVec) {
        line = Settings::trimSpaces(line);

        if(line.size() == 0)
            continue;

        if(line.at(0) == '#')
            continue;

        int sepIndex = line.find(":");

        if(sepIndex == std::string::npos)
            throw FileError(FileError_InvalidNotation);

        std::string propKey = line.substr(0, sepIndex);
        std::string propValue = line.substr(sepIndex + 1);

        // note: valueの両端スペースを削除してから各項目をチェックすること
        propValue = Settings::trimSpaces(propValue);

        if(propKey.size() == 0 || propValue.size() == 0)
            throw FileError(FileError_InvalidNotation);

        if(this->propMap.count(propKey) == 1)
            throw FileError(FileError_InvalidNotation);

        this->propMap[propKey] = propValue;
    }

    this->loaded = true;
}

bool ches::Settings::existsKey(std::string propKey) {
    return this->propMap.count(propKey) == 1;
}

// spec: キーが見つからない場合は空文字を返す
std::string ches::Settings::getValue(std::string propKey) {
    if(!this->existsKey(propKey))
        return "";

    return this->propMap.at(propKey);
}
