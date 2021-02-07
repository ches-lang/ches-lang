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


#include "../../shared/command/command.hpp"

#include "./subcmd/subcmd.hpp"

#include <iostream>
#include <string>


using namespace ches::shared;
using namespace ches::cmd::chesc;


int main(int argc, char *argv[]) {
    try {
        Command cmd(argc, argv, "cmp");
        ChescCommand chescCmd(cmd);
        chescCmd.run();
    } catch(CommandException excep) {
        std::cout << "CommandException (" << excep.type << ") \"" << excep.target << "\"" << std::endl;
    }
}
