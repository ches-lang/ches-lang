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


#include <iostream>
#include <unordered_map>
#include <regex>
#include <string>
#include <vector>

#include "./console.cpp"


using namespace ches::shared;


Console Console::debug = Console("debug");
Console Console::error = Console("error", 31);
Console Console::note = Console("note", 36);
Console Console::warn = Console("warn", 35);

Console::Console() {}

Console::Console(std::string typeName, int typeColor) {
    this->typeName = typeName;
    this->typeColor = typeColor;
}

void Console::print(int title, std::unordered_map<int, std::string> detailMap) {
    // todo: Use langpack value.
    std::string titleStr = std::regex_replace(std::to_string(title), std::regex("\n"), " ");
    std::cout << "\033[" << this->typeColor << "m[" << this->typeName << "]\033[m " << titleStr << std::endl;

    for(auto const [ key, value ] : detailMap) {
        std::string outputValue = std::regex_replace(value, std::regex("\n"), " ");
        // todo: Use langpack value.
        std::cout << "\t" << key << ": " << value << std::endl;
    }

    std::cout << std::endl;
}
