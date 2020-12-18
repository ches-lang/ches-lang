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


#include "../../lib/command/command.hpp"


int main(int argc, char *argv[]) {
    try {
        ches::Command cmd(argc, argv, "cmp", "i");
        cmd.run();
    } catch(ches::CommandError excep) {
        // todo: Consoleを実装してから例外処理を改良する
        std::cout << "command error" << std::endl;
    } catch(std::exception excep) {
        std::cout << "unknown error" << std::endl;
    }
}
