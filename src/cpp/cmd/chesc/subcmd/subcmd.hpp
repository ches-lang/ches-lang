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
#include <regex>
#include <string>
#include <unordered_map>

#include "../../../shared/filemanager/filemanager.hpp"
#include "../../../shared/command/command.hpp"
#include "../../../shared/configulation/configulation.hpp"

#include "./subcmd.cpp"


using namespace ches::shared;
using namespace ches::cmd::chesc;


ChescCommand::ChescCommand(int argc, char* argv[], std::string defaultCmdName = "") {
    std::vector<std::string> args;

    for(int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    this->init(args, defaultCmdName);
}

void ChescCommand::init(std::vector<std::string> args, std::string defaultCmdName = "") {
    try {
        this->cmd = Command(args, defaultCmdName);

        this->cmd.addCommandProc("cmp", ChescCommand::cmd_cmp);
        this->cmd.addCommandProc("help", ChescCommand::cmd_help);
        this->cmd.addCommandProc("set", ChescCommand::cmd_set);
    } catch(CommandException excep) {
        switch(excep.type) {
            case CommandException_CommandNameAlreadyExists:
            Console::error.print("{^command.error.commandNameAlreadyExists}", { { "{^command.word.commandName}", excep.target } }, true);
            break;

            case CommandException_InvalidOptionName:
            Console::error.print("{^command.error.invalidOptionName}", { { "{^command.word.optionName}", excep.target } }, true);
            break;

            case CommandException_OptionNameAlreadyExists:
            Console::error.print("{^command.error.optionNameAlreadyExists}", { { "{^command.word.optionName}", excep.target } }, true);
            break;

            case CommandException_UnexpectedOptionValue:
            Console::error.print("{^command.error.unexpectedOptionValue}", { { "{^command.word.optionValue}", excep.target } }, true);
            break;

            default:
            Console::error.print("{^command.error.unknownCommandError}", true);
            break;
        }
    }
}
