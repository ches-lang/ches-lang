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
#include <iostream>
#include <string>
#include <unordered_map>

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


LangPack::LangPack(std::string filePath) {
    this->filePath = filePath;
    this->loadLangPack();
}

std::string LangPack::getPropValue(std::string propName) {
    if(this->langDataMap.count(propName) == 0)
        return propName;

    return this->langDataMap.at(propName);
}

void LangPack::print() {
    std::cout << "a " << this->langDataMap.size() << std::endl;
}

void LangPack::loadLangPack() {
    if(!std::filesystem::exists(this->filePath))
        throw LangPackException(LangPackException_CouldNotOpenLangFile);

    std::cout << "dircheck" << std::endl;

    if(std::filesystem::is_directory(this->filePath))
        throw LangPackException(LangPackException_CouldNotOpenLangFile);
}
