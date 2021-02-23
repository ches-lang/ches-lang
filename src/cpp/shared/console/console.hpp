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
#include <unordered_map>
#include <regex>
#include <string>
#include <vector>

#include "../configuration/configuration.hpp"

#include "./console.cpp"


using namespace ches::shared;


bool Console::logLimitLoaded = false;

int Console::logLimit = -1;
std::string Console::logLimitSettingName = "log-limit";

int Console::logCount = 0;

Console Console::debug = Console("debug", 33);
Console Console::error = Console("error", 31);
Console Console::note = Console("note", 36);
Console Console::warn = Console("warn", 35);

Console::Console() {}

Console::Console(std::string typeName, int typeColor) {
    this->typeName = typeName;
    this->typeColor = typeColor;
}

void Console::print(std::string title, bool terminateProc) {
    this->print(title, {}, terminateProc);
}

void Console::print(std::string title, std::unordered_map<std::string, std::string> detailMap, bool terminateProc) {
    if(!Console::logLimitLoaded) {
        try {
            Console::logLimit = Console::getLogLimit();
            Console::logLimitLoaded = true;
        } catch(ConfigurationException excep) {
            Console::logLimit = -1;
            // note: 無限ループを防止するため、コンソール出力前に logLimitLoaded を true にする
            Console::logLimitLoaded = true;

            Console::error.print("{^config.setting.error.invalidSettingValue}", { { "{^config.setting.words.settingName}", "log-limit" }, { "{^config.setting.words.settingValue}", excep.target } });
        }
    }

    if(logLimit != -1 && logLimit <= Console::logCount) {
        int tmpLogLimit = Console::logLimit;
        Console::logLimit = -1;

        Console::note.print("{^console.note.logCountHasBeenLimited}", { { "{^console.words.logLimit}", std::to_string(tmpLogLimit) } }, true);

        return;
    }

    Console::logCount++;

    Console::translateText(title);
    std::string outputTitle = std::regex_replace(title, std::regex("\n"), " ");

    std::cout << "\033[" << this->typeColor << "m[" << this->typeName << "]\033[m " << outputTitle << std::endl;

    for(auto const [ key, value ] : detailMap) {
        std::string outputKey = key;
        std::string outputValue = value;

        Console::translateText(outputKey);
        Console::translateText(outputValue);

        outputKey = std::regex_replace(outputKey, std::regex("\n"), " ");
        outputValue = std::regex_replace(outputValue, std::regex("\n"), " ");

        std::cout << "\t" << outputKey << ": " << outputValue << std::endl;
    }

    std::cout << std::endl;

    if(terminateProc)
        exit(-1);
}
