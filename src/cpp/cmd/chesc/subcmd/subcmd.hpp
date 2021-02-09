/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#pragma once


#include "../../../shared/command/command.hpp"

#include "./subcmd.cpp"


using namespace ches::shared;
using namespace ches::cmd::chesc;


ChescCommand::ChescCommand(Command cmd) {
    this->cmd = cmd;
    this->init();
}

void ChescCommand::init() {
    try {
        this->cmd.addCommandProc("cmp", ChescCommand::cmd_cmp);
        this->cmd.addCommandProc("help", ChescCommand::cmd_help);
    } catch(CommandException excep) {
        if(excep.type == CommandException_CommandNameAlreadyExists)
            std::cout << "Duplicated Command Name: " << excep.target << std::endl;
    }
}
