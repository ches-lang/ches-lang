/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


#include <exception>
#include <functional>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../console/console.hpp"
#include "../configulation/configulation.hpp"

#include "./command.cpp"


using namespace ches::shared;


CommandException::CommandException() {}

CommandException::CommandException(CommandExceptionType type) {
    this->type = type;
}

CommandException::CommandException(CommandExceptionType type, std::string target) {
    this->type = type;
    this->target = target;
}


CommandOption::CommandOption() {}


CommandOptionMap::CommandOptionMap() {}


Command::Command() {}

Command::Command(std::vector<std::string> args, std::string defaultCmdName = "") {
    this->defaultCmdName = defaultCmdName;

    this->loadFromArgs(args);
}

void Command::addCommandProc(std::string cmdName, Command::CommandProc proc) {
    if(this->cmdProcMap.count(cmdName) == 1)
        throw CommandException(CommandException_CommandNameAlreadyExists, cmdName);

    this->cmdProcMap[cmdName] = proc;
}

void Command::print() {
    std::unordered_map<std::string, std::string> optionMap;

    for(auto const [ name, option ] : this->cmdOptionMap) {
        std::string joinedOptionValues = "";

        for(int i = 0; i < option.values.size(); i++) {
            joinedOptionValues += option.values.at(i);

            if(i + 1 < option.values.size())
                joinedOptionValues += ", ";
        }

        optionMap[name] = joinedOptionValues;
    }

    Console::debug.print("{^command.word.optionList}", optionMap);
}

void Command::run() {
    if(this->cmdProcMap.count(this->cmdName) == 0)
        throw CommandException(CommandException_UnknownCommandName, this->cmdName);

    try {
        this->cmdProcMap.at(this->cmdName)(*this);
    } catch(...) {
        Console::error.print("{^command.error.uncaughtErrorInSubCommand}", true);
    }
}

void Command::loadFromArgs(std::vector<std::string> args) {
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
            if(value.size() == 1)
                throw CommandException(CommandException_InvalidOptionName, value);

            if(optionName_tmp != "")
                this->cmdOptionMap.addOption(optionName_tmp, optionValues_tmp);

            optionName_tmp = value;
            optionValues_tmp.clear();
            continue;
        }

        if(value.size() >= 2 && value.substr(0, 2) == "\\-")
            value = value.substr(1);

        if(optionName_tmp == "")
            throw CommandException(CommandException_UnexpectedOptionValue, value);

        optionValues_tmp.push_back(value);
    }

    if(optionName_tmp != "")
        this->cmdOptionMap.addOption(optionName_tmp, optionValues_tmp);
}


void SubCommands::run() {
    try {
        this->cmd.run();
    } catch(CommandException excep) {
        switch(excep.type) {
            case CommandException_UnknownCommandName:
            Console::error.print("{^command.error.unknownCommandName}", { { "{^command.word.commandName}", excep.target } }, true);
            break;

            default:
            Console::error.print("{^command.error.unknownCommandError}", true);
            break;
        }
    }
}
