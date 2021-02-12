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


LangPack LangPack::pack = LangPack("/Users/Garnet3106/Desktop/fontokodoj/chestnut/src/data/langpack/ja-jp/");

LangPack::LangPack(std::string absDirPath) {
    this->absDirPath = absDirPath;
    this->loadLangPack();
}

bool LangPack::exists(std::string propName) {
    return this->langDataMap.count(propName);
}

std::string LangPack::get(std::string propName) {
    if(!this->exists(propName))
        return propName;

    return this->langDataMap.at(propName);
}

void LangPack::print() {
    std::cout << "[debug] LangPack Properties ( whole size: " << this->langDataMap.size() << " )" << std::endl;

    for(auto [ key, value ] : this->langDataMap)
        std::cout << "\t" << key << ": " << value << "," << std::endl;

    if(this->langDataMap.size() > 0)
        std::cout << std::endl;
}

void LangPack::loadLangPack() {
    try {
        std::vector<std::string> filePathVec = FileManager::getFilePathsInDirectory(this->absDirPath);

        for(const std::string filePath : filePathVec) {
            std::vector<std::string> lineVec = FileManager::getLines(filePath);

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

                if(this->exists(propName))
                    throw LangPackException(LangPackException_DuplicatedPropName);

                this->langDataMap[propName] = propValue;
            }
        }
    } catch(LangPackException excep) {
        // note: 言語データが利用できないため、switchを使って例外名を取得する
        std::string excepName;

        switch(excep.type) {
            case LangPackException_CouldNotOpenLangFile:
            excepName = "CouldNotOpenLangFile";
            break;

            case LangPackException_DuplicatedPropName:
            excepName = "DuplicatedPropName";
            break;

            case LangPackException_InvalidPropName:
            excepName = "InvalidPropName";
            break;

            case LangPackException_InvalidSyntax:
            excepName = "InvalidSyntax";
            break;

            default:
            excepName = "UnknownLangPackError";
            break;
        }

        std::cout << "error (" << excepName << ") on LangPack::loadLangPack" << std::endl;
    }
}

void LangPack::removeBothSideSpaces(std::string &text) {
    int beforeSpaceLen, behindSpaceLen = 0;

    for(; beforeSpaceLen < text.size(); beforeSpaceLen++)
        if(text.at(beforeSpaceLen) != ' ')
            break;

    for(; behindSpaceLen < text.size(); behindSpaceLen++)
        if(text.at(text.size() - behindSpaceLen - 1) != ' ')
            break;

    if(beforeSpaceLen != 0 && beforeSpaceLen == behindSpaceLen) {
        text = "";
        return;
    }

    text = text.substr(beforeSpaceLen, text.size() - behindSpaceLen - beforeSpaceLen);
}
