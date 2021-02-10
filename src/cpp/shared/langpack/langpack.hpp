/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "./langpack.cpp"


using namespace ches::shared;


LangPackException::LangPackException() {}

LangPackException::LangPackException(LangPackExceptionType type) {
    this->type = type;
}

LangPackException::LangPackException(LangPackExceptionType type, std::string target) {
    this->type = type;
    this->target = target;
}


LangPack::LangPack(std::string absFilePath) {
    this->absFilePath = absFilePath;
    this->loadLangPack();
}

std::string LangPack::getPropValue(std::string propName) {
    if(this->langDataMap.count(propName) == 0)
        return propName;

    return this->langDataMap.at(propName);
}

void LangPack::print() {
    std::cout << "size: " << this->langDataMap.size() << std::endl;

    for(auto [ key, value ] : this->langDataMap)
        std::cout << "\t" << key << ": " << value << "," << std::endl;
}

void LangPack::loadLangPack() {
    LangPackException fileOpenExcep(LangPackException_CouldNotOpenLangFile, this->absFilePath);

    if(!std::filesystem::exists(this->absFilePath))
        throw fileOpenExcep;

    if(std::filesystem::is_directory(this->absFilePath))
        throw fileOpenExcep;

    std::ifstream ifs(this->absFilePath);

    if(!ifs.is_open())
        throw fileOpenExcep;

    std::vector<std::string> lineVec;
    std::string line_tmp;

    while(getline(ifs, line_tmp))
        lineVec.push_back(line_tmp);

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
            throw LangPackException(LangPackException_InvalidSyntax);

        std::string propName = line.substr(0, separatorIndex);
        std::string propValue = line.substr(separatorIndex + 1);

        LangPack::removeBothSideSpaces(propName);
        LangPack::removeBothSideSpaces(propValue);

        if(propName == "")
            throw LangPackException(LangPackException_InvalidPropName);

        if(this->langDataMap.count(propName) == 1)
            throw LangPackException(LangPackException_DuplicatedPropName);

        this->langDataMap[propName] = propValue;
    }
}

void LangPack::removeBothSideSpaces(std::string &str) {
    int beforeSpaceLen, behindSpaceLen = 0;

    for(; beforeSpaceLen < str.size(); beforeSpaceLen++)
        if(str.at(beforeSpaceLen) != ' ')
            break;

    for(; behindSpaceLen < str.size(); behindSpaceLen++)
        if(str.at(str.size() - behindSpaceLen - 1) != ' ')
            break;

    if(beforeSpaceLen != 0 && beforeSpaceLen == behindSpaceLen) {
        str = "";
        return;
    }

    str = str.substr(beforeSpaceLen, str.size() - behindSpaceLen - beforeSpaceLen);
}
