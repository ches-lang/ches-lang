/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#include "../../shared/command/command.hpp"
#include "../../shared/console/console.hpp"

#include "./subcmd/subcmd.hpp"

#include <iostream>
#include <string>


using namespace ches::shared;
using namespace ches::cmd::chesc;


int main(int argc, char *argv[]) {
    try {
        Console::error.print(123, { { 8, "b" }, { 1, "c" } });
        Console::warn.print(123, { { 8, "b" }, { 1, "c" }, { 1, "wefw" } });
        Console::note.print(123, { { 8, "b" }, { 1, "c" } });

        Command cmd(argc, argv, "cmp");
        ChescCommand chescCmd(cmd);
        chescCmd.run();
    } catch(CommandException excep) {
        std::cout << "CommandException (" << excep.type << ") \"" << excep.target << "\"" << std::endl;
    }
}
