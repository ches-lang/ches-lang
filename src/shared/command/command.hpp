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

ches::shared::Command::Command(int argc, char* argv[], std::string defaultCmdName = "") {
    this->defaultCmdName = defaultCmdName;

    std::vector<std::string> args;

    for(int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    this->loadFromArgs(args);
}

ches::shared::Command::Command(std::vector<std::string> args, std::string defaultCmdName = "") {
    this->defaultCmdName = defaultCmdName;

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
    if(args.size() < 2) {
        this->cmdName = this->defaultCmdName;
        return;
    }

    std::string optionName_tmp = "";
    std::vector<std::string> optionValues_tmp;

    int index = 2;

    if(args.at(1).size() == 0 || args.at(1).at(0) != '-') {
        this->cmdName = args.at(1);
    } else {
        this->cmdName = this->defaultCmdName;
        index--;
    }

    for(; index < args.size(); index++) {
        std::string value = args.at(index);

        if(value.size() == 0)
            continue;

        if(value.at(0) == '-') {
            if(optionName_tmp != "")
                this->optionMap.addOption(optionName_tmp, optionValues_tmp);

            optionName_tmp = value;
            optionValues_tmp.clear();
            continue;
        }

        if(optionName_tmp == "")
            throw CommandException(CommandException_UnexpectedOptionValue);

        optionValues_tmp.push_back(value);
    }

    if(optionName_tmp != "")
        this->optionMap.addOption(optionName_tmp, optionValues_tmp);
}
