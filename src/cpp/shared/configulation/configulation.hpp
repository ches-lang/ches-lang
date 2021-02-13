/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../filemanager/filemanager.hpp"

#include "./configulation.cpp"


using namespace ches::shared;


ConfigulationException::ConfigulationException() {}

ConfigulationException::ConfigulationException(ConfigulationExceptionType type) {
    this->type = type;
}

ConfigulationException::ConfigulationException(ConfigulationExceptionType type, std::string target) {
    this->type = type;
    this->target = target;
}


Configulation Configulation::settings = Configulation("/Users/Garnet3106/Desktop/fontokodoj/chestnut/src/data/langpack/ja-jp/");
Configulation Configulation::langPack = Configulation("/Users/Garnet3106/Desktop/fontokodoj/chestnut/src/data/langpack/ja-jp/");

Configulation::Configulation() {}

Configulation::Configulation(std::string path) {
    this->path = path;
    this->loadData();
}

bool Configulation::exists(std::string key) {
    return this->dataMap.count(key);
}

std::string Configulation::get(std::string key) {
    if(!this->exists(key))
        return key;

    return this->dataMap.at(key);
}

void Configulation::print() {
    std::cout << "[debug] Configulation Properties ( whole size: " << this->dataMap.size() << " )" << std::endl;

    for(auto [ key, value ] : this->dataMap)
        std::cout << "\t" << key << ": " << value << "," << std::endl;

    if(this->dataMap.size() > 0)
        std::cout << std::endl;
}

void Configulation::loadData() {
    std::vector<std::string> lineVec;

    try {
        if(!FileManager::exists(this->path))
            throw FileManagerException(FileManagerException_PathNotFound, this->path);

        if(FileManager::isDirectory(this->path)) {
            std::vector<std::string> filePathVec = FileManager::getFilePathsInDirectory(this->path);

            for(std::string filePath : filePathVec) {
                std::vector<std::string> lineVec_tmp = FileManager::getLines(filePath);

                for(std::string line : lineVec_tmp)
                    lineVec.push_back(line);
            }
        } else {
            lineVec = FileManager::getLines(this->path);
        }
    } catch(FileManagerException excep) {
        // note: 言語データが利用できないため、switchを使ってエラー名を取得する

        std::string errorName;

        switch(excep.type) {
            case FileManagerException_FileUnopenable:
            errorName = "FileUnopenable";
            break;

            case FileManagerException_NotFilePath:
            errorName = "NotFilePath";
            break;

            case FileManagerException_PathNotFound:
            errorName = "PathNotFound";
            break;

            default:
            errorName = "UnknownFileManagerError";
            break;
        }

        std::cout << "error (FileError: " << errorName << "; " << excep.target << ") on Configulation::loadData()" << std::endl;
        exit(-1);
    }

    try {
        for(std::string line : lineVec) {
            if(line.size() == 0)
                continue;

            if(line.at(0) == '#')
                continue;

            int separatorIndex = 0;

            for(separatorIndex = 0; separatorIndex < line.size(); separatorIndex++)
                if(line.at(separatorIndex) == '=')
                    break;

            if(line.size() == separatorIndex)
                throw ConfigulationException(ConfigulationException_InvalidSyntax);

            std::string propName = line.substr(0, separatorIndex);
            std::string propValue = line.substr(separatorIndex + 1);

            Configulation::removeBothSideSpaces(propName);
            Configulation::removeBothSideSpaces(propValue);

            if(propName == "")
                throw ConfigulationException(ConfigulationException_InvalidPropName);

            if(this->exists(propName))
                throw ConfigulationException(ConfigulationException_DuplicatedPropName);

            this->dataMap[propName] = propValue;
        }
    } catch(ConfigulationException excep) {
        // note: 言語データが利用できないため、switchを使ってエラー名を取得する

        std::string errorName;

        switch(excep.type) {
            case ConfigulationException_DuplicatedPropName:
            errorName = "DuplicatedPropName";
            break;

            case ConfigulationException_InvalidPropName:
            errorName = "InvalidPropName";
            break;

            case ConfigulationException_InvalidSyntax:
            errorName = "InvalidSyntax";
            break;

            default:
            errorName = "UnknownConfigulationError";
            break;
        }

        std::cout << "error (" << errorName << "; " << excep.target << ") on Configulation::loadConfigulation" << std::endl;
        exit(-1);
    }
}
