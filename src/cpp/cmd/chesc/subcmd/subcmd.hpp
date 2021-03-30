/*
 * Ches 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/ches-lang/
 *
 * Copyright © 2019-2021 Garnet3106 All rights reserved.
 */


#pragma once


#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

#include "../../../shared/command/command.hpp"
#include "../../../shared/configuration/configuration.hpp"
#include "../../../shared/filemanager/filemanager.hpp"

#include "../../../compiler/compiler/compiler.hpp"

#include "./subcmd.cpp"


using namespace ches::cmd::chesc;
using namespace ches::shared;


ChescCommand::ChescCommand(int argc, char* argv[], std::string defaultCmdName = "") noexcept {
    std::vector<std::string> args;

    for(int i = 0; i < argc; i++)
        args.push_back(argv[i]);

    this->init(args, defaultCmdName);
}

void ChescCommand::init(std::vector<std::string> args, std::string defaultCmdName = "") noexcept {
    try {
        this->cmd = Command(args, defaultCmdName);

        this->cmd.addCommandProc("cmp", ChescCommand::cmd_cmp);
        this->cmd.addCommandProc("help", ChescCommand::cmd_help);
        this->cmd.addCommandProc("set", ChescCommand::cmd_set);
    } catch(CommandException excep) {
        switch(excep.type) {
            case CommandException_DuplicatedCommandName:
            Console::error.print("{^command.error.duplicatedCommandName}", { { "{^command.words.commandName}", excep.target } }, true);
            break;

            case CommandException_InvalidOptionName:
            Console::error.print("{^command.error.invalidOptionName}", { { "{^command.words.optionName}", excep.target } }, true);
            break;

            case CommandException_DuplicatedOptionName:
            Console::error.print("{^command.error.duplicatedOptionName}", { { "{^command.words.optionName}", excep.target } }, true);
            break;

            case CommandException_UnexpectedOptionValue:
            Console::error.print("{^command.error.unexpectedOptionValue}", { { "{^command.words.optionValue}", excep.target } }, true);
            break;

            default:
            Console::error.print("{^command.error.unknownCommandError}", true);
            break;
        }
    }
}
