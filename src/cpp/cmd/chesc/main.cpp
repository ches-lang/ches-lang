/*
 * Chestnut 0.0.0
 *
 * GitHub Repository:
 *     https://github.com/Garnet3106/chestnut/
 *
 * Copyright © 2020-2021 Garnet3106 All rights reserved.
 */


#include "../../shared/langpack/langpack.hpp"
#include "../../shared/command/command.hpp"
#include "../../shared/console/console.hpp"

#include "./subcmd/subcmd.hpp"

#include <iostream>
#include <string>


using namespace ches::shared;
using namespace ches::cmd::chesc;


int main(int argc, char *argv[]) {
    try {
        LangPack pack("/Users/Garnet3106/Desktop/fontokodoj/chestnut/src/data/langpack/ja-jp/command.lang");
        pack.print();

        Command cmd(argc, argv, "cmp");
        ChescCommand chescCmd(cmd);
        chescCmd.run();
    } catch(CommandException excep) {
        std::cout << "CommandException (" << excep.type << ") \"" << excep.target << "\"" << std::endl;
    } catch(LangPackException excep) {
        std::cout << "LangPackException (" << excep.type << ") \"" << excep.target << "\"" << std::endl;
    }
}
