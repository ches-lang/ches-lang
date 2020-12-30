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


#include <unordered_map>

#include "../filemanager/filemanager.hpp"

#include "./datastruct.cpp"



// excep: FileError
void ches::PropMap::load(std::string filePath) {
    std::vector<std::string> lineVec;

    try {
        lineVec = FileManager::readTextLines(filePath);
    } catch(FileError excep) {
        throw excep;
    }

    for(std::string line : lineVec) {
        line = trim_string_spaces(line);

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
        propValue = trim_string_spaces(propValue);

        if(propKey.size() == 0 || propValue.size() == 0)
            throw FileError(FileError_InvalidNotation);

        if(this->exists(propKey))
            throw FileError(FileError_InvalidNotation);

        (*this)[propKey] = propValue;
    }

    this->loaded = true;
}


ches::LangPack ches::LangPack::data;
std::string ches::LangPack::langCode = "en";


ches::Settings ches::Settings::data;
