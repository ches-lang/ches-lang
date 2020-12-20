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

#include <functional>
#include <unordered_map>
#include <vector>

#include "../filemanager/filemanager.hpp"

#include "./langpack.cpp"


ches::LangPackError::LangPackError() {}

ches::LangPackError::LangPackError(LangPackErrorType type) {
    this->type = type;
}


ches::LangPack::LangPack() {}

bool ches::LangPack::existsKey(std::string propKey) {
    return this->propMap.count(propKey) == 1;
}

std::string ches::LangPack::getValue(std::string propKey) {
    if(!this->existsKey(propKey))
        return "";

    return this->propMap.at(propKey);
}

// spec: 記述エラーを含む行は無視される
// excep: LangPackError
void ches::LangPack::load(std::string filePath) {
    std::vector<std::string> lineVec;

    try {
        lineVec = FileManager::readTextLines(filePath);
    } catch(FileError excep) {
        throw LangPackError(LangPackError_FileLoadFailed);
    }

    for(std::string line : lineVec) {
        line = LangPack::trimSpaces(line);

        if(line.size() == 0)
            continue;

        if(line.at(0) == '#')
            continue;

        int sepIndex = line.find(" ");

        if(sepIndex == std::string::npos)
            throw LangPackError(LangPackError_InvalidNotation);

        std::string propKey = line.substr(0, sepIndex);
        std::string propValue = line.substr(sepIndex + 1);

        // note: valueの両端スペースを削除してから各項目をチェックすること
        propValue = LangPack::trimSpaces(propValue);

        if(propKey.size() == 0 || propValue.size() == 0)
            throw LangPackError(LangPackError_InvalidNotation);

        if(this->existsKey(propKey))
            throw LangPackError(LangPackError_DuplicatedPropKey);

        this->propMap[propKey] = propValue;
    }
}
