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


#include <exception>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "command.cpp"


ches::shared::CommandException::CommandException() {}

ches::shared::CommandException::CommandException(CommandExceptionType type) {
    this->type = type;
}

ches::shared::CommandException::CommandException(CommandExceptionType type, std::string target) {
    this->type = type;
    this->target = target;
}


ches::shared::CommandOption::CommandOption() {}


ches::shared::CommandOptionMap::CommandOptionMap() {}


ches::shared::Command::Command() {}

ches::shared::Command::Command(int argc, char* argv[], std::string defaultCmdName = "", std::string defaultOptionName = "") {
    this->defaultCmdName = defaultCmdName;
    this->defaultOptionName = defaultOptionName;

    std::vector<std::string> args;

    for(int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    this->loadFromArgs(args);
}

ches::shared::Command::Command(std::vector<std::string> args, std::string defaultCmdName = "", std::string defaultOptionName = "") {
    this->defaultCmdName = defaultCmdName;
    this->defaultOptionName = defaultOptionName;

    this->loadFromArgs(args);
}

void ches::shared::Command::print() {
    std::cout << "[deb] optName: " << this->cmdName << std::endl;

    int mapIndex = 0;

    for(auto [ optionName, optionValue ] : this->optionMap) {
        std::cout << "    " << optionName << ": ";

        std::vector<std::string> optionValueVec = this->optionMap.at(optionName).values;
        int optionIndex = 0;

        for(std::string optionValue : optionValueVec) {
            std::cout << optionValue;

            optionIndex++;

            if(optionIndex < optionValueVec.size())
                std::cout << ", ";
        }

        std::cout << std::endl;

        mapIndex++;
    }

    if(mapIndex >= this->optionMap.size())
        std::cout << std::endl << std::endl;
}

void ches::shared::Command::loadFromArgs(std::vector<std::string> args) {
    if(args.size() <= 1) {
        this->cmdName = this->defaultCmdName;
        this->optionMap.addOption(this->defaultOptionName);
        return;
    }

    this->cmdName = args.at(1);

    std::string optionName_tmp = this->defaultOptionName;
    std::vector<std::string> optionValues_tmp;

    for(int i = 2; i < args.size(); i++) {
        std::string value = args.at(i);

        if(value.size() == 0)
            continue;

        if(value.at(0) == '-') {
            if(optionName_tmp != "")
                this->optionMap.addOption(optionName_tmp, optionValues_tmp);

            optionName_tmp = value;
            optionValues_tmp.empty();
            continue;
        }

        optionValues_tmp.push_back(value);
    }

    this->optionMap.addOption(optionName_tmp, optionValues_tmp);
}
