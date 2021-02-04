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

#include <iostream>
#include <string>


int main(int argc, char *argv[]) {
    try {
        ches::shared::Command cmd(argc, argv, "cmp", "-i");
        cmd.print();
    } catch(ches::shared::CommandException excep) {
        std::cout << "CommandException (" << excep.type << ") \"" << excep.target << "\"" << std::endl;
    }
}
