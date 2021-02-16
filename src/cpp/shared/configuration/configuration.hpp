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
#include <utility>
#include <vector>

#include "../filemanager/filemanager.hpp"

#include "./configuration.cpp"


using namespace ches::shared;


ConfigurationException::ConfigurationException() {}

ConfigurationException::ConfigurationException(ConfigurationExceptionType type) {
    this->type = type;
}

ConfigurationException::ConfigurationException(ConfigurationExceptionType type, std::string target) {
    this->type = type;
    this->target = target;
}


std::string Configuration::homeDirEnvName = "CHES_HOME";

Configuration Configuration::settings;
Configuration Configuration::langPack;

Configuration::Configuration() {}


void Configuration::edit(std::unordered_map<std::string, std::string> editedOptionMap) {
    for(const auto [ key, value ] : editedOptionMap)
        if(!this->exists(key))
            throw ConfigurationException(ConfigurationException_UnknownPropName);

    std::vector<std::string> lineVec;

    try {
        lineVec = FileManager::getLines(this->path);
    } catch(FileManagerException excep) {
        throw excep;
    }

    for(int i = 0; i < lineVec.size(); i++) {
        std::pair<std::string, std::string> prop;

        try {
            prop = Configuration::toPropPair(lineVec.at(i));
        } catch(ConfigurationException excep) {
            throw excep;
        }

        if(prop == (std::pair<std::string, std::string>){})
            continue;

        if(editedOptionMap.count(prop.first) == 1) {
            std::string newValue = editedOptionMap.at(prop.first);

            if(newValue.find("\n") != std::string::npos)
                throw ConfigurationException(ConfigurationException_InvalidPropValue, newValue);

            lineVec.at(i) = prop.first + "=" + newValue;
        }
    }

    try {
        FileManager::writeLines(this->path, lineVec);
    } catch(FileManagerException excep) {
        throw excep;
    }
}

bool Configuration::exists(std::string key) {
    return this->dataMap.count(key);
}

std::string Configuration::get(std::string key) {
    if(!this->exists(key))
        return key;

    return this->dataMap.at(key);
}

void Configuration::loadData(std::string path) {
    this->path = path;

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
        throw excep;
    }

    try {
        for(std::string line : lineVec) {
            std::pair<std::string, std::string> prop = Configuration::toPropPair(line);

            if(prop == (std::pair<std::string, std::string>){})
                continue;

            if(this->exists(prop.first))
                throw ConfigurationException(ConfigurationException_DuplicatedPropName);

            this->dataMap[prop.first] = prop.second;
        }
    } catch(ConfigurationException excep) {
        throw excep;
    }
}

void Configuration::print() {
    std::cout << "[debug] Configuration Properties ( whole size: " << this->dataMap.size() << " )" << std::endl;

    for(auto [ key, value ] : this->dataMap)
        std::cout << "\t" << key << ": " << value << "," << std::endl;

    if(this->dataMap.size() > 0)
        std::cout << std::endl;
}
